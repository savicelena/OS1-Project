//
// Created by os on 7/31/23.
//

#include "../h/scheduler.hpp"

List Scheduler::readyCoroutineQueue;

TCB *Scheduler::get()
{
    return readyCoroutineQueue.removeFirst();
}

void Scheduler::put(TCB *ccb)
{
    readyCoroutineQueue.addLast(ccb);
}
