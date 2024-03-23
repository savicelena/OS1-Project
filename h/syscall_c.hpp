//
// Created by os on 7/31/23.
//

#ifndef _syscall_c
#define _syscall_c

#include "../lib/hw.h"
#include "memoryAllocator.hpp"
#include "_sem.hpp"

void* mem_alloc (size_t size);
int mem_free (void*);

class TCB;
typedef TCB* thread_t;
int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
);
int thread_exit ();
void thread_dispatch ();
void thread_join (thread_t handle);

class _sem;
typedef _sem* sem_t;
int sem_open (
        sem_t* handle,
        unsigned init
);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);

const int EOF = -1;
char getc ();
void putc (char);


#endif //_syscall_c

//C API dat u okviru projekta