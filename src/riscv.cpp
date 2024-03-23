//
// Created by os on 7/31/23.
//

#include "../h/riscv.hpp"

void Riscv::popSppSpie()
{
    //povratak iz sistemskog rezima
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrapSystem(){
    uint64 scause = r_scause();
    uint64 code;
    __asm__ volatile("mv %0, a0" : "=r"(code));

    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
        uint64 sstatus = r_sstatus();
        uint64 sepc = r_sepc() + 4;
        //sepc ukazuje na samu instrukciju ecall pa ga je neophodno uvecati

        if(code == 0x01){
            size_t blknum;
            __asm__ volatile("mv %0, a1" : "=r"(blknum));
            void* allocated = memoryAllocator::mem_alloc(blknum);
            //void* allocated = __mem_alloc(blknum);
            __asm__ volatile("mv a0, %0" : : "r"(allocated));
        }else if(code == 0x02){
            void* ptr;
            __asm__ volatile("mv %0, a1" : "=r"(ptr));
            int res = memoryAllocator::mem_free(ptr);
            //int res = __mem_free(ptr);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }else if(code == 0x11) {
            TCB** handle;
            TCB::Body body;
            void* arg;
            //upisivanje sa steka u registre t3, t4 i t5
            __asm__ volatile("ld t3, 11*8(fp)");
            __asm__ volatile("ld t4, 12*8(fp)");
            __asm__ volatile("ld t5, 13*8(fp)");

            __asm__ volatile("mv %0, t3" : "=r"(handle));
            __asm__ volatile("mv %0, t4" : "=r"(body));
            __asm__ volatile("mv %0, t5" : "=r"(arg));
            int res = TCB::createThread(handle, body, arg, false);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        } else if(code == 0x12) {
            int res = TCB::thr_exit();
            __asm__ volatile("mv a0, %0" : : "r"(res));
        } else if(code == 0x13){
            TCB::dispatch();
        } else if(code == 0x14) {
            TCB* handle;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            TCB::thread_join(handle);
        }else if(code == 0x21) {
            _sem** handle;
            unsigned init;

            __asm__ volatile("ld t3, 11*8(fp)");
            __asm__ volatile("ld t4, 12*8(fp)");

            __asm__ volatile("mv %0, t3" : "=r"(handle));
            __asm__ volatile("mv %0, t4" : "=r"(init));
            int res = _sem::sem_create(handle, init);
            __asm__ volatile("mv a0, %0" : : "r"(res));

        }else if(code == 0x22) {
            _sem* handle;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            int res = _sem::sem_close(handle);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }else if(code == 0x23) {
            _sem* id;
            __asm__ volatile("mv %0, a1" : "=r"(id));
            int res = _sem::wait(id);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }else if(code == 0x24) {
            _sem* id;
            __asm__ volatile("mv %0, a1" : "=r"(id));
            int res = _sem::signal(id);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }else if (code == 0x31) {
            time_t time;
            __asm__ volatile("mv %0, a1" : "=r"(time));
            int res = TCB::addToSleep(time);
            __asm__ volatile("mv a0, %0" : : "r"(res));
        }else if(code == 0x41) {
            //getc
            char c = UtilConsole::getObj()->getIn();
            __asm__ volatile("mv a0, %0" : : "r"(c));
        }else if (code == 0x42) {
            //putc
            char c;
            __asm__ volatile("mv %0, a1" : "=r"(c));
            UtilConsole::getObj()->putOut(c);}


        w_sstatus(sstatus);
        w_sepc(sepc);
        //mc_sip(SIP_SSIP);
    }else if(scause == 0x0000000000000002UL){
        uint64 sscause = r_scause();
        uint64 sepc = r_sepc();
        uint64 stval = r_stval();
        printInt(sscause);
        printString(" ");
        printInt(sepc);
        printString(" ");
        printInt(stval);
        printString(" ");
        printString("ILEGALNA INSTRUKCIJA");
        printString("\n");
        thread_dispatch();
    }else if(scause == 0x0000000000000005UL){
        printString("NEDOZVOLJENA ADRESA CITANJA");
        printString(" ");
        printInt(r_sepc());
        printString("  ");
        printInt(r_scause());
        printString(" ");
        printInt(r_stval());
        printString("\n");
    }else if(scause == 0x0000000000000007UL){
       printString("NEDOZVOLJENA ADRESA UPISA");
        printString(" ");
        printInt(r_sepc());
        printString(" ");
        printInt(r_scause());
        printString(" ");
        printInt(r_stval());
        printString("\n");
    }
}
void Riscv::handleSupervisorTrapTime(){
// interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
    //postavljanje da je prekid obradjen
    mc_sip(SIP_SSIP);
    //prolazak kroz uspavane niti i smanjivanje perioda spavanja
    //ukoliko je period spavananja kod niti istekao ona se vraca u Scheduler
    Elem* tek = TCB::sleeping.head;
    while(tek){
        if(tek->sleepPeriod > 0) tek->sleepPeriod--;
        Elem* elem = tek;
        tek = tek->next;

        if(elem->sleepPeriod == 0){
            TCB* thr = TCB::sleeping.removeElem(elem);
            Scheduler::put(thr);
            thr->setAsleep(false);
        }
    }

    TCB::timeSliceCounter++;
    uint64 gt = TCB::running->getTimeSlice();
    if (TCB::timeSliceCounter >= gt)
    {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        TCB::timeSliceCounter = 0;
        TCB::dispatch();
        w_sstatus(sstatus);
        w_sepc(sepc);
    }
}
void Riscv::handleSupervisorTrapConsole(){
    //console_handler();
    if(plic_claim() == CONSOLE_IRQ){
        //uzimanje podataka od ulaznog uredjaja tehnikom prozivanja
        while(*(char*)CONSOLE_STATUS & CONSOLE_RX_STATUS_BIT){
            //if(UtilConsole::getObj()->getBuffIn()->buffFul()) break;
            char c = *(char*)CONSOLE_RX_DATA;
            UtilConsole::getObj()->putIn(c);
        }

        plic_complete(CONSOLE_IRQ);
    }
}

void Riscv::handleUnknownTrap(){
    printString("Unknown trap!");
}


