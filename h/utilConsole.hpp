//
// Created by os on 8/11/23.
//

#ifndef OS1_UTILCONSOLE_HPP
#define OS1_UTILCONSOLE_HPP

#include "buff.hpp"
#include "memoryAllocator.hpp"
#include "syscall_cpp.hpp"

class Buff;

class UtilConsole{

    static UtilConsole* obj;

    Buff* buffIn;
    Buff* buffOut;

public:

    static UtilConsole* getObj();

    char getIn();
    char getOut();
    void putIn(char c);
    void putOut(char c);

    Buff* getBuffIn() const {return buffIn;}
    Buff* getBuffOut() const {return buffOut;}


};

#endif //OS1_UTILCONSOLE_HPP
