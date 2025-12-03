#ifndef _PRIORITY_
#define _PRIORITY_

#include <stdbool.h>
#include "../entity/position.h"

typedef struct{
    int prio;
    int cost;
    position* node;
    tree_node* way;
}data;

typedef struct {
    int size;
    data** heap;
} priority_list;


data* new_data(int,position*,tree_node*,int);
void delete_data(data*);

priority_list* new_priority_list(int);
bool empty_priority_list(priority_list*);

void swap(int,int,priority_list*);
void percolate_up(int,priority_list*);
int choose_son(int,priority_list*);
void percolate_down(int,priority_list*);

void insert(data*,priority_list*);
data* remove_root(priority_list*);

bool is_in_priority(priority_list*,position*,int);

void delete_priority_list(priority_list*);

void adjust_priority_list(priority_list*,position*,position);

#endif