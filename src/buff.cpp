//
// Created by os on 8/11/23.
//

#include "../h/buff.hpp"


void Buff::makeBuff(Buff *buff) {
    buff->size = 512;
    buff->head = 0;
    buff->tail = 0;
    buff->curr = 0;
    _sem::sem_create(&buff->spaceAvailable, buff->size);
    _sem::sem_create(&buff->itemAvailable, 0);
    buff->arr = (char*)memoryAllocator::mem_alloc(buff->size * sizeof(char));

}

char Buff::takeItem() {
    //uzimanje podatka ukoliko postoji i signalizacija da ima slobodnog mesta
    _sem::wait(itemAvailable);
    char c = arr[head];
    head = (head+1)%size;
    curr--;
    _sem::signal(spaceAvailable);
    return c;
}

void Buff::putItem(char c) {
    //cekanje na slobodno mesto za stavljanje podatka i signalizacija da postoji podatak
    _sem::wait(spaceAvailable);
    arr[tail] = c;
    tail = (tail+1)%size;
    curr++;
    _sem::signal(itemAvailable);
}

Buff::~Buff() {
    _sem::sem_close(itemAvailable);
    _sem::sem_close(spaceAvailable);
    memoryAllocator::mem_free(arr);
}

bool Buff::buffFul() {
    if(curr == size) return true;
    return false;
}

bool Buff::buffEmpty() {
    if(curr == 0) return true;
    return false;
}