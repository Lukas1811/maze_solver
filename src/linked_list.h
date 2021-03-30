#include <stdio.h>
#include <memory.h>

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct LLNode{
    struct LLNode* previous;
    struct LLNode* next;
    void* element;
}LLNode;

LLNode* LL_create(void* element)
{
    LLNode* node = (LLNode*)malloc(sizeof(LLNode));
    node->next = NULL;
    node->previous = NULL;
    node->element = element;
    return node;
}  

__always_inline LLNode* LL_next(LLNode* node)
{
    //assert(node != NULL);
    if(node->next == NULL){
        return NULL;
    }
    return (LLNode*)node->next;
}

__always_inline LLNode* LL_previous(LLNode* node)
{
    return (LLNode*)node->previous;
}

LLNode* LL_append(LLNode* list_arg, void* element)
{
    if(list_arg == NULL){
        return LL_create(element);
    }

    LLNode* list = list_arg;
    // go to end of list
    while (list->next != NULL){
        list = list->next;
    }
    
    LLNode* new = LL_create(element);
    list->next = new;
    new->previous = list;
    return new;
}

LLNode* LL_swap_next(LLNode* node)
{
    assert(node != NULL);
    assert(node->next != NULL);
    void* next_element = LL_next(node)->element;
    node->next->element = node->element;
    node->element = next_element;


    return LL_next(node);
}

LLNode* LL_first(LLNode* list)
{
    if(list == NULL)
    {
        return NULL;
    }

    while(LL_previous(list) != NULL)
    {
        list = LL_previous(list);
    }

    return list;
}

void LL_delete(LLNode* list)
{
    // go to start of list 
    assert(list != NULL);
    while (list->previous != NULL)
    {
        list = list->previous;
    }

    while (list->next != NULL)
    {
        list = list->next;
        free(list->previous->element);
        free(list->previous);
    }
}

// Pops a node out of the list and returns the next node if not NULL otherwise return the previous one
LLNode* LL_remove(LLNode* node)
{
    LLNode* prev = LL_previous(node);
    LLNode* next = LL_next(node);

    node->next = NULL;
    node->previous = NULL;

    free(node);

    if(prev != NULL)
    {
       if(next != NULL)
       {
           next->previous = prev;
           prev->next = next;
       }else
       {
           prev->next = NULL;
       }
       return prev;
    }else
    {   
        if(next != NULL)
        {
            next->previous = NULL;
            return next;
        }
    }

    return NULL;
}

bool LL_contained(LLNode* list, LLNode* element)
{
    list = LL_first(list);

    while(list != NULL)
    {
        if(list == element)
        {
            return true;
        }
        list = LL_next(list);
    }
    return false;
}

#endif