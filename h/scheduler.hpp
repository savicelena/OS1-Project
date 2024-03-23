//
// Created by os on 7/31/23.
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

#include "list.hpp"

class TCB;
class List;

class Scheduler
{
private:
    static List readyCoroutineQueue;

public:
    static TCB *get();

    static void put(TCB *tcb);

};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_1_SYNCHRONOUS_SCHEDULER_HPP

//interfejs i implementacija preuzeti sa 7. vezbi