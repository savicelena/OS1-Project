//
// Created by os on 7/31/23.
//

#include "../h/syscall_c.hpp"

void* mem_alloc (size_t size){

    __asm__ volatile("mv a1, %0": : "r"(size));
    //kod sistemskog poziva
    __asm__ volatile ("mv a0, %0" : : "r"(0x01));

    __asm__ volatile("ecall");

    void* allocated;
    //povratna vrednost u a0
    __asm__ volatile("mv %0, a0" : "=r"(allocated));
    return allocated;

}

int mem_free (void* ptr){
    __asm__ volatile("mv a1, %0" : : "r"(ptr));
    __asm__ volatile("mv a0, %0": : "r"(0x02));

    __asm__ volatile ("ecall");

    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

void thread_dispatch(){
    __asm__ volatile("mv a0, %0" : : "r"(0x13));
    __asm__ volatile("ecall");
}

int thread_create(thread_t* handle, void(*start_routine)(void*),void* arg){
    __asm__ volatile("mv a3, a2");
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");

    __asm__ volatile("mv a0, %0" : : "r"(0x11));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

int thread_exit (){
    __asm__ volatile("mv a0, %0" : : "r"(0x12));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

void thread_join (thread_t handle){
    __asm__ volatile("mv a1, %0": : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x14));
    __asm__ volatile("ecall");
}

int sem_open (sem_t* handle,unsigned init){
    __asm__ volatile("mv a2, a1");
    __asm__ volatile("mv a1, a0");

    __asm__ volatile("mv a0, %0" : : "r"(0x21));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

int sem_close (sem_t handle){
    __asm__ volatile("mv a1, %0": : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x22));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}
int sem_wait (sem_t id){
    __asm__ volatile("mv a1, %0": : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(0x23));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

int sem_signal (sem_t id){
    __asm__ volatile("mv a1, %0": : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(0x24));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

int time_sleep (time_t time){
    __asm__ volatile("mv a1, %0": : "r"(time));
    __asm__ volatile("mv a0, %0" : : "r"(0x31));
    __asm__ volatile("ecall");
    int res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

char getc (){
    __asm__ volatile("mv a0, %0" : : "r"(0x41));
    __asm__ volatile("ecall");
    char res;
    __asm__ volatile ("mv %0, a0" : "=r"(res));
    return res;
}

void putc (char c){
    __asm__ volatile("mv a1, %0": : "r"(c));
    __asm__ volatile("mv a0, %0" : : "r"(0x42));
    __asm__ volatile("ecall");
}
