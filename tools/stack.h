#ifndef _stack_
#define _stack_

#include <stdbool.h>
#include "../entity/position.h"

typedef struct s_e stack_elt;
struct s_e{
    position* pos;
    int time;
    stack_elt* next;
};

typedef struct{
    stack_elt* top;
}stack;

stack_elt* new_stack_elt(position*,int);
void delete_stack_elt(stack_elt*);

stack* new_stack();
void delete_stack(stack*);

void enstack(stack_elt*,stack*);
stack_elt* destack(stack*);

bool empty_stack(stack*);
bool is_in_stack(position*,int,stack*);

void adjust_stack(stack*,int);

#endif