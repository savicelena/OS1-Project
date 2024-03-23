//
// Created by os on 7/31/23.
//

#include "../h/list.hpp"

void List::addFirst(TCB *data)
{
    Elem *elem = (Elem*)memoryAllocator::mem_alloc(sizeof(Elem));
    elem->data= data;
    elem->sleepPeriod = 0;
    elem->next = nullptr;
    head = elem;
    if (!tail) { tail = head; }
}

void List::addLast(TCB *data)
{
    Elem *elem = (Elem*)memoryAllocator::mem_alloc(sizeof(Elem));
    elem->data= data;
    elem->sleepPeriod = 0;
    elem->next = nullptr;
    if (tail)
    {
        tail->next = elem;
        tail = elem;
        tail->next = nullptr;
    } else
    {
        head = tail = elem;

    }
}

TCB* List::removeFirst()
{
    if (!head) { return nullptr; }

    Elem *elem = head;
    head = head->next;
    if (!head) { tail = nullptr; }

    TCB *ret = elem->data;

    memoryAllocator::mem_free(elem);
    return ret;
}

TCB* List::peekFirst()
{
    if (!head) { return nullptr; }
    return head->data;
}

TCB* List::removeLast()
{
    if (!head) { return nullptr; }

    Elem *prev = nullptr;
    for (Elem *curr = head; curr && curr != tail; curr = curr->next)
    {
        prev = curr;
    }

    Elem *elem = tail;
    if (prev) { prev->next = nullptr; }
    else { head = nullptr; }
    tail = prev;

    TCB *ret = elem->data;

    memoryAllocator::mem_free(elem);
    return ret;
}

TCB* List::peekLast()
{
    if (!tail) { return nullptr; }
    return tail->data;
}

TCB* List::removeElem(Elem *el) {
    Elem* tek = head;
    Elem* prev = nullptr;
    while(tek != el){
        prev = tek;
        tek = tek->next;
    }
    //ako ne postoji prethodni brise se prvi
    if(!prev){
        head = tek->next;
    }else{
        prev->next = tek->next;
    }
    if(tek == tail){
        tail = prev;
    }

    TCB* ret = tek->data;

    memoryAllocator::mem_free(tek);
    return ret;
}