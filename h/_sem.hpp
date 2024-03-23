//
// Created by os on 8/1/23.
//


#ifndef PROJECT_BASE_V1_1__SEM_HPP
#define PROJECT_BASE_V1_1__SEM_HPP

#include "list.hpp"
#include "syscall_c.hpp"
#include "memoryAllocator.hpp"

class List;

class _sem{
    List* blocked;
    _sem* handle;
    unsigned value;

public:
    //_sem(unsigned val = 1);
    static int sem_create(_sem** handle, unsigned int init);
    static int sem_close(_sem* handle);
    ~_sem();

    static int wait(_sem* id);
    static int signal(_sem* id);

    unsigned getVal() const;
    void setVal(unsigned val);


protected:
    void block(_sem* id);
    void unblock(_sem* id);

    int cnt = 0;

};

#endif //PROJECT_BASE_V1_1__SEM_HPP
