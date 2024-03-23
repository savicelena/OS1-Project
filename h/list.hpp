//
// Created by os on 7/31/23.
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP

#include "memoryAllocator.hpp"
#include "tcb.hpp"
#include "../lib/mem.h"
#include "syscall_c.hpp"
#include "riscv.hpp"


class TCB;

struct Elem
{
    TCB* data;
    unsigned long sleepPeriod;
    Elem *next;

    Elem(TCB *data, Elem *next) : data(data), sleepPeriod(0), next(next){}
};

class List
{
private:
    Elem *head, *tail;

public:

    friend class Riscv;
    friend class TCB;
    friend class _sem;

    List() : head(0), tail(0) {};

    List(const List &) = delete;

    List &operator=(const List &) = delete;

    void addFirst(TCB *data);

    void addLast(TCB *data);

    TCB *removeFirst();

    TCB *peekFirst();

    TCB *removeLast();

    TCB *peekLast();

    TCB* removeElem(Elem* el);
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_LIST_HPP

//implementacija liste vecim delom preuzeta sa 7. vezbi