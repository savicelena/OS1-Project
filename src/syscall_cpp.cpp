//
// Created by os on 8/1/23.
//

#include "../h/syscall_cpp.hpp"

#include "../h/memoryAllocator.hpp"



void* operator new (size_t n){
    return memoryAllocator::mem_alloc(n);
}

void operator delete (void* p){
    memoryAllocator::mem_free(p);
}

void* operator new[] (size_t n){
    return memoryAllocator::mem_alloc(n);
}

void operator delete[] (void* p){
    memoryAllocator::mem_free(p);
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
}

Thread::Thread() {
    //thread_create(&myHandle, &thread_wrapper, this);

}

void Thread::thread_wrapper(void *arg) {
    //neophodno je napraviti wrapper funkciju zato sto funkcija run ne prima argumente, a nit se pravi nad f-jom koja prima void*
    Thread* thr = (Thread*)arg;
    thr->run();
}

Thread::~Thread() {
    myHandle->setFinished(true);
    memoryAllocator::mem_free(myHandle);
}

int Thread::start() {
    thread_create(&myHandle, &thread_wrapper, this);
    return 0;
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t){
    int res = time_sleep(t);
    return res;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    int res = sem_wait(myHandle);
    return res;
}

int Semaphore::signal() {
    int res = sem_signal(myHandle);
    return res;
}

void PeriodicThread::terminate() {
    thread_dispatch();
}

PeriodicThread::PeriodicThread(time_t period) : period(period){}


char Console::getc() {
    char c = ::getc();
    return c;
}

void Console::putc(char c) {
    ::putc(c);
}