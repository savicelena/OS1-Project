//
// Created by os on 7/30/23.
//

#include "../lib/console.h"
#include "../h/memoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/workers.hpp"
#include "../h/_sem.hpp"

size_t toBlk(size_t size){
    size_t needed = size + memoryAllocator::sizeofBlock;
    size_t blknum = needed/MEM_BLOCK_SIZE;
    if(needed%MEM_BLOCK_SIZE) blknum++;
    return blknum;
}

extern void userMain();

int main(){

   //u registar stvec se stavlja adresa iv tabele i u najniza dva bita vrednost jedan - vektorski rezim
   //ulaz u iv tabeli za spoljasnje prekide se dobija: BASE + 4 * br. spoljasnjeg prekida
    Riscv::w_stvec((uint64) ((uint64)&Riscv::ivtable | 0x1));
    //__asm__ volatile("csrw sie, %[sie]" : : [sie]"r"(0x220));

    //formiranje niza niti - idle, userMain i za konzolu
    TCB** threads = (TCB**) memoryAllocator::mem_alloc(3*sizeof(TCB*));
    int r = TCB::createThread(&threads[0], nullptr, nullptr, true);
    TCB::running = threads[0];
    r = thread_create(&threads[1], (void(*)(void*))&userMain, nullptr);
    r = TCB::createThread(&threads[2], workerConsumer, nullptr, true);


    //dozvoljavaju se prekidi
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    if(r < 0) thread_exit();
    while(!threads[1]->isFinished()){
        thread_dispatch();
    }

    return 0;

}
