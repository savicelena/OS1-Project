//
// Created by os on 7/31/23.
//

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/utilConsole.hpp"

void workerConsumer(void* ptr){
    while(true){
        //Buff* buff = UtilConsole::getObj()->getBuffOut();
        //if(buff->buffEmpty()) continue;
        while(*(char*)CONSOLE_STATUS & CONSOLE_TX_STATUS_BIT){
            *(char*)CONSOLE_TX_DATA = UtilConsole::getObj()->getOut();
        }
    }

}

