//
// Created by os on 7/31/23.
//

#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "riscv.hpp"
#include "list.hpp"
#include "syscall_c.hpp"
#include "../lib/hw.h"
class List;

class TCB{
public:
    TCB() {}
    using Body = void(*)(void*);
    static int createThread(TCB** handle, Body body, void* arg, bool system);

    bool isFinished() const{
        return finished;
    }
    void setFinished(bool finished){
        this->finished=finished;
    }

    uint64 getTimeSlice() const{
        return timeSlice;
    }

    void setAsleep(bool asleep){
        this->asleep = asleep;
    }

    bool getAsleep(){
        return this->asleep;
    }

    static int thr_exit();

    static void thread_join(TCB* handle);

    static TCB* running;
    static void dispatch();

    static int addToSleep(unsigned long time);

    void setBlocked(bool bl) {blocked = bl;}

    ~TCB();

    friend class Riscv;

    static List threads;
    static List sleeping;
    List* waiting;


    _sem* blockedOn = nullptr;

private:

    void initialize(Body body, void* arg, uint64 timeSlice);

    struct Context{
        uint64 ra; //gde se vracamo nakon sto predamo procesor
        uint64 sp; //pokazuje dokle je nit na svom steku stavila podatke
        //sve ostale registre cuvamo na steku niti
    };

    //TCB* handle;
    void* arg;
    Body body;
    uint64* stack;
    Context context;
    bool finished = false;
    uint64 timeSlice;
    bool blocked;
    bool asleep;
    bool system;

    static void threadWrapper();

    static void contextSwitch(Context* oldContext, Context* newContext);

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 32*DEFAULT_STACK_SIZE;
    static uint64 constexpr TIME_SLICE = DEFAULT_TIME_SLICE;


};


#endif //PROJECT_BASE_V1_1_TCB_HPP

//delovi implementacije su preuzeti iz 7. vezbi