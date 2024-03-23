//
// Created by os on 8/11/23.
//

#ifndef OS1_BUFF_HPP
#define OS1_BUFF_HPP

#include "_sem.hpp"

class _sem;

class Buff{

    uint64 size;
    _sem* spaceAvailable;
    _sem* itemAvailable;
    uint64 head, tail;
    char* arr;
    uint64 curr;



public:
    //Buff();
    static void makeBuff(Buff* buff);
    ~Buff();
    char takeItem();
    void putItem(char c);

    bool buffFul();
    bool buffEmpty();
};

#endif //OS1_BUFF_HPP

//ideja preuzeta iz knjige "Osnovi operativnih sistema", Dragan Milicev