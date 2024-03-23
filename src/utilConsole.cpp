//
// Created by os on 8/11/23.
//

#include "../h/utilConsole.hpp"

UtilConsole* UtilConsole::obj = nullptr;


UtilConsole* UtilConsole::getObj() {
    if(obj == nullptr){
        obj = (UtilConsole*)memoryAllocator::mem_alloc(sizeof(UtilConsole));
        obj->buffIn = (Buff*)memoryAllocator::mem_alloc(sizeof(Buff));
        Buff::makeBuff(obj->buffIn);
        obj->buffOut = (Buff*)memoryAllocator::mem_alloc(sizeof(Buff));
        Buff::makeBuff(obj->buffOut);
    }
    return obj;
}

void UtilConsole::putOut(char c) {
    buffOut->putItem(c);
}

void UtilConsole::putIn(char c) {
    buffIn->putItem(c);
}

char UtilConsole::getIn() {
    char c = buffIn->takeItem();
    return c;
}

char UtilConsole::getOut() {
    char c = buffOut->takeItem();
    return c;
}