#include <stdio.h>
#include <stdlib.h>
#include "priority.h"

data* new_data(int prio, position* pos, tree_node* old, int cost){
    data* res = malloc(sizeof(data));
    res->prio = prio;
    res->node = pos;
    res->way = old;
    res->cost = cost;
    return res;
}

void delete_data(data* e){
    delete_position(e->node);
    free(e);
}

priority_list* new_priority_list(int n){
    priority_list* list = malloc(sizeof(priority_list));
    data** tab = calloc(n+1, sizeof(data*));
    list->size = n;
    list->heap = tab;
    list->heap[0] = new_data(0,0,0,0);
    return list;
}

bool empty_priority_list(priority_list* list){
    return list->heap[0]->cost == 0;
}

void delete_priority_list(priority_list* list){
    while (!empty_priority_list(list)){
        delete_data(remove_root(list));
    }
    free(list->heap[0]);
    free(list->heap);
    free(list);
}

void swap(int i,int j,priority_list* list){
    data* tmp = list->heap[i];
    list->heap[i] = list->heap[j];
    list->heap[j] = tmp;
}

void percolate_up(int k,priority_list* list){
    if(k>1){
        int m = k/2;
        if(list->heap[m]->prio > list->heap[k]->prio){
            swap(m,k,list);
            percolate_up(m,list);
        }
    }
}

int choose_son(int k, priority_list* list){
    if(k*2<list->heap[0]->cost){
        if(list->heap[2*k]->prio < list->heap[2*k+1]->prio){
            return 2*k;
        }
        else{
            return 2*k+1;
        }
    }
    else{
        if(2*k == list->heap[0]->cost){
            return 2*k;
        }
        else{
            return -1;
        }
    }
}

void percolate_down(int k, priority_list* list){
    int m = choose_son(k,list);
    if(m>-1 && list->heap[m]->prio < list->heap[k]->prio){
        swap(m,k,list);
        percolate_down(m,list);
    }
}

void insert(data* e,priority_list* list){
    if(list->heap[0]->cost < list->size){
        list->heap[0]->cost++;
        list->heap[list->heap[0]->cost] = e;
        percolate_up(list->heap[0]->cost,list);
    }
    else{
        list->heap = realloc(list->heap, (2*list->size+1)*sizeof(data*));
        list->size = list->size*2;
        insert(e,list);
    }
}

data* remove_root(priority_list* list){
    data* res = NULL;
    if(list->heap[0]->cost > 0){
        res = list->heap[1];
        list->heap[1] = list->heap[list->heap[0]->cost];
        list->heap[0]->cost--;
        percolate_down(1,list);
    }
    return res;
}

bool is_in_priority(priority_list* list, position* pos, int cost) {
    for(int i = 1; i <= list->heap[0]->cost; i++){
        if(equals(list->heap[i]->node,pos) && list->heap[i]->cost <= cost) return true;
    }
    return false;
}

void adjust_priority_list(priority_list* list, position* pos, position end){
    int n = list->heap[0]->cost;
    priority_list* tmp = new_priority_list(list->size);

    for(int i = 1; i<=n; i++){
        if(list->heap[i]->way->start != NULL 
            && equals(&(list->heap[i]->way->start->pos),pos)){
            data* e = list->heap[i];
            e->cost--;
            e->prio = e->cost+range(e->node,&end);

            e->way->start->prev = NULL;
            e->way->start = new_start_tree_node(e->way);

            insert(e,tmp);
        }
        else{
            delete_tree_node(list->heap[i]->way);
            delete_data(list->heap[i]);
        }
    }

    free(list->heap[0]);
    free(list->heap);
    list->heap = tmp->heap;
    free(tmp);
}