//
// Created by os on 7/30/23.
//


#include "../h/memoryAllocator.hpp"

memoryAllocator::Block* memoryAllocator::fhead = nullptr;
memoryAllocator::Block* memoryAllocator::ohead = nullptr;
size_t memoryAllocator::sizeofBlock = sizeof(memoryAllocator::Block);

void memoryAllocator::allocatorInitialization() {
    if(!memoryAllocator::fhead && !memoryAllocator::ohead){
        fhead = (Block*)HEAP_START_ADDR;
        fhead->size = (char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR - sizeof(Block);
        fhead->next = nullptr;
    }
}

void* memoryAllocator::mem_alloc(size_t size) {
    //inicijalizacija glava potrebnih listi
    allocatorInitialization();
    size_t needed = size + memoryAllocator::sizeofBlock;
    size_t blknum = needed/MEM_BLOCK_SIZE;
    if(needed%MEM_BLOCK_SIZE) blknum++;
    //odredjivanje velicine koja se treba alocirati
    size_t toAllocate = blknum * MEM_BLOCK_SIZE;
    Block* block = fhead;
    Block* prev = nullptr;
    //trazenje slobodnog bloka dovoljne velicine za alokaciju
    while(block->size < toAllocate + sizeof(Block)){
        prev = block;
        block = block->next;
        if(!block){
            return nullptr;
        }
    }
    //ukoliko je prvi blok odgovarajuci onda se glava liste pomera
    //u suprotnom se vrsi preulancavanje liste
    if(!prev){
        if(!fhead->next) {
            fhead = (Block*)((char*)fhead + sizeof(Block) + toAllocate);
            fhead->size = block->size - sizeof(Block) - toAllocate;
            fhead->next = nullptr;
            block->size = toAllocate;
        }else{
            fhead = fhead->next;
        }
    }else{
        prev->next = block->next;
        if(!prev->next){
            prev->next = (Block*)((char*)block + sizeof(Block) + toAllocate);
            prev->next->size = block->size - sizeof(Block) - toAllocate;
            prev->next->next = nullptr;
            block->size = toAllocate;
        }
    }
    //block->size = toAllocate;
    Block* occupied = ohead;
    prev = nullptr;
    if(!occupied){
        ohead = block;
        block->next = nullptr;
        return (char*)block + sizeof(Block);
    }
    //odrzavanje zauzetih blokova po adresi rastuce
    while(occupied && occupied < block){
        prev = occupied;
        occupied = occupied->next;
    }
    //ulancavanje u listu zauzetih blokova
    if(!prev){
        block->next = ohead;
        ohead = block;
    }else{
        prev->next = block;
        block->next = occupied;
    }
    //return (char*)block + sizeof(Block);
    return (char*)block + memoryAllocator::sizeofBlock;
}

int memoryAllocator::mem_free(void *ptr) {
    if(!ohead) return -1;
    Block* block = (Block*)ohead;
    Block* toFree = (Block*)((char*)ptr - sizeof(Block));
    Block* prev = nullptr;
    //provera da li se zahteva oslobadjanje bloka koji je prethodno zauzet funkcijom mem_alloc
    while(block != toFree){
        prev = block;
        block = block->next;
        if(!block) return -1;
    }
    //preulancavanje liste zauzetih blokova
    if(!prev){
        ohead = block->next;
    }else{
        prev->next = block->next;
    }
    //ulancavanje u listu slobodnih blokova
    block = fhead;
    prev = nullptr;
    while(block < toFree){
        prev = block;
        block = block->next;
    }
    if(!prev){
        toFree->next = fhead;
        fhead = toFree;
    }else{
        prev->next = toFree;
        toFree->next = block;
    }
    //pokusaj spajavanja sa slobodnim fragmentom iza ili ispred bloka koji se oslobadja
    if((char*)toFree + toFree->size + sizeof(Block) == (char*)block){
        toFree->size += block->size + sizeof(Block);
        toFree->next = block->next;
    }
    if(prev && ((char*)prev + prev->size + sizeof(Block)) == (char*)toFree){
        prev->size += toFree->size + sizeof(Block);
        prev->next = toFree->next;
    }
    return 0;
}

memoryAllocator::~memoryAllocator(){
    if(fhead) fhead->next = nullptr;
    fhead = nullptr;
    if(ohead) ohead->next= nullptr;
    ohead = nullptr;

}