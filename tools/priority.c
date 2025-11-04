#include <stdio.h>
#include <stdlib.h>
#include "priority.h"

data* new_data(int prio, position* pos, tree_node* old, int cout){
    data* res = malloc(sizeof(data));
    res->prio = prio;
    res->node = pos;
    res->way = old;
    res->cout = cout;
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
    list->tas = tab;
    list->tas[0] = new_data(0,0,0,0);
    return list;
}

bool empty_priority_list(priority_list* list){
    return list->tas[0]->cout == 0;
}

void delete_priority_list(priority_list* list){
    while (!empty_priority_list(list)){
        delete_data(remove_rac(list));
    }
    free(list->tas[0]);
    free(list->tas);
    free(list);
}

void swap(int i,int j,priority_list* list){
    data* tmp = list->tas[i];
    list->tas[i] = list->tas[j];
    list->tas[j] = tmp;
}

void percolate_up(int k,priority_list* list){
    if(k>1){
        int m = k/2;
        if(list->tas[m]->prio > list->tas[k]->prio){
            swap(m,k,list);
            percolate_up(m,list);
        }
    }
}

int choose_son(int k, priority_list* list){
    if(k*2<list->tas[0]->cout){
        if(list->tas[2*k]->prio < list->tas[2*k+1]->prio){
            return 2*k;
        }
        else{
            return 2*k+1;
        }
    }
    else{
        if(2*k == list->tas[0]->cout){
            return 2*k;
        }
        else{
            return -1;
        }
    }
}

void percolate_down(int k, priority_list* list){
    int m = choose_son(k,list);
    if(m>-1 && list->tas[m]->prio < list->tas[k]->prio){
        swap(m,k,list);
        percolate_down(m,list);
    }
}

void insert(data* e,priority_list* list){
    if(list->tas[0]->cout < list->size){
        list->tas[0]->cout++;
        list->tas[list->tas[0]->cout] = e;
        percolate_up(list->tas[0]->cout,list);
    }
    else{
        list->tas = realloc(list->tas, (2*list->size+1)*sizeof(data*));
        list->size = list->size*2;
        insert(e,list);
    }
}

data* remove_rac(priority_list* list){
    data* res = NULL;
    if(list->tas[0]->cout > 0){
        res = list->tas[1];
        list->tas[1] = list->tas[list->tas[0]->cout];
        list->tas[0]->cout--;
        percolate_down(1,list);
    }
    return res;
}

bool is_in_priority(priority_list* list, position* pos, int cout) {
    for(int i = 1; i <= list->tas[0]->cout; i++){
        if(equals(list->tas[i]->node,pos) && list->tas[i]->cout <= cout) return true;
    }
    return false;
}

void adjust_priority_list(priority_list* list, position* pos, position end){
    int n = list->tas[0]->cout;
    priority_list* tmp = new_priority_list(list->size);

    for(int i = 1; i<=n; i++){
        if(list->tas[i]->way->start != NULL 
            && equals(&(list->tas[i]->way->start->pos),pos)){
            data* e = list->tas[i];
            e->cout--;
            e->prio = e->cout+range(e->node,&end);

            e->way->start->prev = NULL;
            e->way->start = new_start_tree_node(e->way);

            insert(e,tmp);
        }
        else{
            delete_tree_node(list->tas[i]->way);
            delete_data(list->tas[i]);
        }
    }

    free(list->tas[0]);
    free(list->tas);
    list->tas = tmp->tas;
    free(tmp);
}