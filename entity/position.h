#ifndef _POSITION_
#define _POSITION_

#include <stdbool.h>

typedef struct {
    int x;
    int y;
} position;

typedef struct {
    int size;
    position** tab;
} list_position;

position* new_position(int,int);
position* copy_position(position*);
void delete_position(position*);

list_position* new_list_position(int);
list_position* copy_list_position(list_position*);//à faire
void delete_list_position(list_position*);

void moveto(position*,int,int);

bool equals(position*,position*);

float range(position*,position*);

typedef struct q_e queue_elt;
struct q_e {
    position pos;
    queue_elt* next;
    queue_elt* prev;
};

typedef struct {
    queue_elt* end;
    queue_elt* start;
} queue_position;

queue_elt* new_queue_elt(position);
void delete_queue_elt(queue_elt*);

queue_position* new_queue_position();
void delete_queue_position(queue_position*);

void enqueue(queue_position*,queue_elt*);
queue_elt* dequeue(queue_position*);

queue_position* copy_queue(queue_position*);

typedef struct t_n tree_node;
struct t_n {
    tree_node** nexts;
    int nb_nexts;
    int nb_deleted;
    tree_node* prev;
    int name;
    tree_node* start;
    position pos;
};

tree_node* new_tree_node(position,tree_node*,tree_node*);
void add_next_tree_node(tree_node*,tree_node*);
void delete_tree_node(tree_node*);
void delete_tree_node_rac(tree_node*);

tree_node* new_start_tree_node(tree_node*);

#endif