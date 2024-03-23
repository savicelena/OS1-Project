//
// Created by os on 8/1/23.
//

#ifndef _syscall_cpp
#define _syscall_cpp

#include "syscall_c.hpp"
#include "tcb.hpp"
//#include "../lib/console.h"
#include "../test/printing.hpp"

using size_t = decltype(sizeof(0));

void* operator new (size_t);
void operator delete (void*);
void* operator new[] (size_t);
void operator delete[] (void*);

class TCB;
typedef TCB* thread_t;


class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
    Thread ();

protected:

    virtual void run () {}

private:
    static void thread_wrapper(void* arg);
    thread_t myHandle;
    void (*body)(void*); void* arg;
};

class _sem;
typedef _sem* sem_t;
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate ();

protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}

private:
    time_t period;
};

class Console {
public:
    static char getc ();
    static void putc (char);
};

#endif

//CPP API dat u okviru projekta