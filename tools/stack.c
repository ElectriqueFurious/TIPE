#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

stack_elt* new_stack_elt(position* pos,int time){
    stack_elt* elt = malloc(sizeof(stack_elt));
    elt->pos = pos;
    elt->time = time;
    elt->next = NULL;
    return elt;
}

void delete_stack_elt(stack_elt* elt){
    delete_position(elt->pos);
    free(elt);
}

stack* new_stack(){
    stack* s = malloc(sizeof(stack));
    s->top = NULL;
    return s;
}

void delete_stack(stack* s){
    while(!empty_stack(s)){
        delete_stack_elt(destack(s));
    }
    free(s);
}

void enstack(stack_elt* elt,stack* s){
    elt->next = s->top;
    s->top = elt;
}

stack_elt* destack(stack* s){
    if(!empty_stack(s)){
        stack_elt* elt = s->top;
        s->top = elt->next;
        return elt;
    }
    return NULL;
}

bool empty_stack(stack* s){
    return s->top == NULL;
}

bool is_in_stack(position* pos, int time, stack* s){
    stack_elt* elt = s->top;
    while (elt != NULL){
        if(equals(pos,elt->pos) && time == elt->time){
            return true;
        }
        elt = elt->next;
    }
    return false;
}

void adjust_stack(stack* s, int time){
    stack_elt* elt = s->top;
    while(elt != NULL){
        stack_elt* next = elt->next;
        if(next != NULL && next->time < time){
            elt->next = next->next;
            delete_stack_elt(next);
        }
        else{
            elt = next;
        }
    }
    if(s->top != NULL && s->top->time < time){
        stack_elt* elt = s->top;
        s->top = s->top->next;
        delete_stack_elt(elt);
    }
}