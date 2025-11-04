#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "position.h"
#include "../scene/scene.h"

position* new_position(int x, int y){
    position* pos = malloc(sizeof(position));
    pos->x = x;
    pos->y = y;
    return pos;
} 

position* copy_position(position* pos){
    return new_position(pos->x, pos->y);
}

void delete_position(position* pos){
    free(pos);
}

list_position* new_list_position(int nb){
    list_position* list = malloc(sizeof(list_position));
    position** tab = calloc(nb, sizeof(position*));
    list->size = nb;
    list->tab = tab;
    return list;
}

void delete_list_position(list_position* list_pos){
    for(int i=0; i<list_pos->size; i++){
        delete_position(list_pos->tab[i]);
    }
    free(list_pos->tab);
    free(list_pos);
}

void moveto(position* pos, int x, int y){
    pos->x = x;
    pos->y = y;
}

bool equals(position* pos1,position* pos2){
    if(pos1->y == pos2->y && pos1->x == pos2->x){
        return true;
    }
    else{
        return false;
    }
}

float range(position* pos1,position* pos2){
    int dx = (pos1->x - pos2->x);
    int dy = (pos1->y - pos2->y);
    return sqrt(dx*dx+dy*dy);
}

queue_elt* new_queue_elt(position pos){
    queue_elt* res = malloc(sizeof(queue_elt));
    res->next = NULL;
    res->prev = NULL;
    res->pos = pos;
    return res;
}

void delete_queue_elt(queue_elt* elt){
    free(elt);
}

queue_position* new_queue_position(){
    queue_position* res = malloc(sizeof(queue_position));
    res->end = NULL;
    res->start = NULL;
    return res;
}

void delete_queue_position(queue_position* q){
    while (q->end != NULL){
        delete_queue_elt(dequeue(q));
    }
    free(q);    
}

void enqueue(queue_position* q, queue_elt* elt){
    if(q->start != NULL){
        elt->next = q->start;
        q->start->prev = elt;
        q->start = elt;
    }
    else{
        q->end = elt;
        q->start = elt;
    }
}

queue_elt* dequeue(queue_position* q){
    queue_elt* res;
    if(q->end == q->start){
        res = q->start;
        q->start = NULL;
        q->end = NULL;
    }
    else{
        res = q->end;
        res->prev->next = NULL;
        q->end = res->prev;
    }
    res->prev = NULL;
    res->next = NULL;
    return res;
}

queue_position* copy_queue(queue_position* q){
    queue_position* res = new_queue_position();
    if(q != NULL && q->end != NULL){
        queue_elt* elt = q->end;
        while (elt != q->start){
            enqueue(res,new_queue_elt(elt->pos));
            elt = elt->prev;
        }
        enqueue(res,new_queue_elt(elt->pos));
    }
    return res;
}

tree_node* new_tree_node(position pos, tree_node* start, tree_node* prev){
    tree_node* res = malloc(sizeof(tree_node));
    res->nexts = calloc(5,sizeof(tree_node*));
    res->nb_nexts = 0;
    res->nb_deleted = 0;
    res->prev = prev;
    res->start = start;
    res->pos = pos;
    return res;
}

void add_next_tree_node(tree_node* node, tree_node* next){
    if(node->nb_nexts < 5){
        node->nexts[node->nb_nexts] = next;
        next->name = node->nb_nexts;
        node->nb_nexts ++;
    }
}

void swap_t_n(tree_node* node,int i, int j){
    node->nexts[i]->name = j;
    node->nexts[j]->name = i;
    tree_node* tmp = node->nexts[i];
    node->nexts[i] = node->nexts[j];
    node->nexts[j] = tmp;
}

void delete_tree_node(tree_node* node){
    for(int i=0; i<node->nb_nexts; i++){
        node->nexts[i]->prev = NULL;
        delete_tree_node_rac(node->nexts[i]);
    }
    if(node->prev != NULL){
        tree_node* prev = node->prev;
        swap_t_n(prev,node->name,prev->nb_nexts-1);
        prev->nb_deleted ++;
        prev->nb_nexts --;
        if(prev->nb_deleted >= 5){
            delete_tree_node(prev);
        }
        free(node->nexts);
        free(node);
    }
}

void delete_tree_node_rac(tree_node* rac){
    delete_tree_node(rac);
    free(rac->nexts);
    free(rac);
}

tree_node* new_start_tree_node(tree_node* node){
    tree_node* current = node;
    tree_node* prev = current->prev;
    if(prev == NULL || prev->prev == NULL){
        return NULL;
    }
    while(prev != NULL && prev->prev != NULL){
        current = prev;
        prev = current->prev;
    }
    return current;
}