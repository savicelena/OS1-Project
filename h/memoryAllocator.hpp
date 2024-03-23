//
// Created by os on 7/30/23.
//

#ifndef OS1_MEMORYALLOCATOR_HPP
#define OS1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"
class memoryAllocator{

    //memoryAllocator();

    struct Block{
        size_t size;
        Block* next;
    };

    //memoryAllocator* obj;
    //glava slobodnih fragmenata
    static Block* fhead;
    //glava zauzetih fragmenata
    static Block* ohead;


public:

    static size_t sizeofBlock;
    static void allocatorInitialization();
    static void* mem_alloc(size_t blknum);
    static int mem_free(void* ptr);

    ~memoryAllocator();
};

#endif //OS1_MEMORYALLOCATOR_HPP

//ideja za resenje dobijena na osnovu zadataka za prvi kolokvijum