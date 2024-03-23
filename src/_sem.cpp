//
// Created by os on 8/1/23.
//


#include "../h/_sem.hpp"
#include "../h/riscv.hpp"

_sem::~_sem() {
    memoryAllocator::mem_free(handle);
}


unsigned _sem::getVal() const{
    return value;
}

void _sem::setVal(unsigned val) {
    value = val;
}


int _sem::sem_create(_sem **handle, unsigned int init) {
    _sem* hand = (_sem*)memoryAllocator::mem_alloc(sizeof(_sem));
    if(!hand) return -1;
    *handle = hand;
    hand->handle = hand;
    hand->value = init;
    hand->cnt = 0;
    hand->blocked = (List*) memoryAllocator::mem_alloc(sizeof(List));
    hand->blocked->head = nullptr;
    hand->blocked->tail = nullptr;
    return 0;
}

int _sem::sem_close(_sem *handle) {
    if(!handle) return -1;
    Elem* tek = handle->blocked->head;
    //prilikom zatvaranja semafora blokirane niti se vracaju u Scheduler
    while(tek){
        tek->data->setBlocked(false);
        Scheduler::put(tek->data);
        tek = tek->next;
    }
    memoryAllocator::mem_free(handle);
    return 0;
}

int _sem::wait(_sem* id){
    if(!id) return -1;
    //promenljiva cnt broji koliko je niti blokirano
    if(id->value == 0){
        id->cnt++;
        id->block(id);
    }else{
        id->value = id->value - 1;
    }
    return 0;
}

int _sem::signal(_sem *id) {
    if(!id) return -1;
    //ako postoje blokirane niti jedna ce se odblokirati
    if(id->cnt > 0 && id->value == 0){
        id->cnt--;
        id->unblock(id);
    }else{
        id->value = id->value + 1;
    }
    return 0;
}

void _sem::block(_sem* id){
    //tekuca nit se dodaje u listu blokiranih niti i setuje se nje flag
    id->blocked->addLast(TCB::running);
    TCB::running->setBlocked(true);
    TCB::running->blockedOn = id;
    //tekuca nit postaje neka druga jer je prethodna blokirana
    Riscv::pushRegisters();
    TCB::dispatch();
    Riscv::popRegisters();

}

void _sem::unblock(_sem* id) {
    //uklanja se jedna nit iz liste blokiranih, flag se postavlja na false i vraca se u Scheduler
    TCB* thr = id->blocked->removeFirst();
    thr->setBlocked(false);
    thr->blockedOn = nullptr;
    Scheduler::put(thr);
}