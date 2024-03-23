//
// Created by os on 7/31/23.
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/scheduler.hpp"




TCB *TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;
List TCB::threads;
List TCB::sleeping;



int TCB::createThread(TCB** handle, Body body, void* arg, bool system){
    TCB* hand = (TCB*) memoryAllocator::mem_alloc(sizeof(TCB));
    if(!hand) return -1;
    hand->initialize(body, arg, TIME_SLICE);
    *handle = hand;
    hand->system = system;
    threads.addLast(*handle);

    return 0;
}


void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished() && !old->blocked && !old->asleep){
        Scheduler::put(old);
    }
    running = Scheduler::get();


    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper() {
    //postavljanje rezima iz koga se desio skok pre izlaska iz sistemskog rezima
    if(running->system){
        Riscv::w_sstatus((uint64)Riscv::r_sstatus() | 0x100);
    }else{
        Riscv::w_sstatus((uint64)Riscv::r_sstatus() & 0xfffffffffffffeff);
    }
    Riscv::popSppSpie();
    running->body(running->arg);

    TCB::thr_exit();
}

void TCB::initialize(Body body, void* arg, uint64 timeSlice){
    this->body = body;
    this->stack = (uint64*)memoryAllocator::mem_alloc(sizeof(uint64)*STACK_SIZE);


    this->context.sp = stack != nullptr ? (uint64)&stack[STACK_SIZE] : 0;
    this->context.ra = (uint64)&threadWrapper;

    this->finished = false;
    this->blocked = false;
    this->asleep = false;
    this->timeSlice = timeSlice;
    this->arg = arg;

    waiting = (List*) memoryAllocator::mem_alloc(sizeof(List));
    this->waiting->head = nullptr;
    this->waiting->tail = nullptr;

    if (body != nullptr) {
        Scheduler::put(this);
    }
}

int TCB::thr_exit() {
    TCB::running->finished = true;
    //niti koje su cekale se vracaju u Scheduler
    while(TCB::running->waiting->peekFirst()){
        Scheduler::put(TCB::running->waiting->removeFirst());
    }
    thread_dispatch();
    return 0;
}

void TCB::thread_join(TCB *handle) {
    if(handle->isFinished()) return;
    handle->waiting->addLast(TCB::running);
    TCB* old = running;
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);
}

int TCB::addToSleep(unsigned long time) {
    Elem *elem = (Elem*)memoryAllocator::mem_alloc(sizeof(Elem));
    //Elem *elem = (Elem*)__mem_alloc(sizeof(Elem));
    if(elem == nullptr) return -1;
    elem->data= TCB::running;
    elem->sleepPeriod = time;
    elem->next = nullptr;
    //dodavanje niti u listu uspavanih
    if (sleeping.tail)
    {
        sleeping.tail->next = elem;
        sleeping.tail = elem;
        sleeping.tail->next = nullptr;
    } else
    {
        sleeping.head = sleeping.tail = elem;
    }
    TCB::running->setAsleep(true);
    TCB::dispatch();
    //NE MENJAJ ! ! !
    return 0;
}

TCB::~TCB(){
    Elem* tek = waiting->head;
    while(tek){
        Elem* old = tek;
        tek = tek->next;
        Scheduler::put(old->data);
        memoryAllocator::mem_free(old);
    }
    memoryAllocator::mem_free(waiting);
    memoryAllocator::mem_free(stack);
}