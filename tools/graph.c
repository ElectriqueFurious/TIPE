#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"
#include "out.h"

list_position* neighbors2(scene_object* scene, position* pos, int height, int width) {
    list_position* list = new_list_position(5);
    int cpt = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (abs(dx * dy) != 1) {
                int new_x = pos->x + dx;
                int new_y = pos->y + dy;

                if (new_x >= 0 && new_x < width && new_y >= 0 
                    && new_y < height && scene->grid[new_y][new_x] == scene->init) {
                    list->tab[cpt++] = new_position(new_x, new_y);
                }
            }
        }
    }

    list->size = cpt;
    return list;
}

list_position* neighbors(scene_object* scene, position* pos, int height, int width){
    list_position* list = new_list_position(5);
    int cpt=0;

    int positionsx[5] = {pos->x,pos->x,pos->x,pos->x+1,pos->x-1};
    int positionsy[5] = {pos->y-1,pos->y,pos->y+1,pos->y,pos->y};

    for(int i = 0;i<5;i++){
        int new_x = positionsx[i];
        int new_y = positionsy[i];
        int condition = new_x >= 0 && new_x < width && new_y >= 0 && new_y < height;
        if (condition && scene->grid[new_y][new_x] == scene->init) {
            list->tab[cpt] = new_position(positionsx[i],positionsy[i]);
            cpt++;
        }
    }

    list->size = cpt;
    return list;
}

position* A_star(scene_object** scene_list, int time,int accuracy, position* default_end, 
                    stack* closedList, priority_list* openList){
    int height = scene_list[0]->height;
    int width = scene_list[0]->width;
    position end = *default_end;

    while (!empty_priority_list(openList)){
        data* u = openList->heap[1];
        end.x = u->way->pos.x;

        if((u->node->x == end.x && u->node->y == end.y) || u->cost >= accuracy-1){
            position* res = copy_position(u->node);
            if(u->way->start != NULL){
                delete_position(res);
                res = copy_position(&(u->way->start->pos));
            }
            return res;
        }

        u = remove_root(openList);
        list_position* voisins = neighbors(scene_list[(time+u->cost+1)%accuracy],u->node,height,width);
        bool have_neighbors = false;

        for(int i = 0; i<voisins->size;i++){
            position* elt = voisins->tab[i];
            if(!(is_in_stack(elt,u->cost+1,closedList) ||
                 is_in_priority(openList,elt,u->cost+1))){
                have_neighbors = true;
                elt = copy_position(elt);
                int d = range(&end,elt);
                tree_node* start = u->way->start;
                if(start == NULL && u->way->prev != NULL){
                    start = u->way;
                }
                tree_node* new_way = new_tree_node(*elt,start,u->way);
                add_next_tree_node(u->way,new_way);
                data* v = new_data(u->cost+1+d,elt,new_way,u->cost+1);
                insert(v,openList);
            }
        }
        if(!have_neighbors){
            delete_tree_node(u->way);//relieves memory
        }
        delete_list_position(voisins);
        enstack(new_stack_elt(copy_position(u->node),u->cost),closedList);
        delete_data(u);
    }
    return NULL;
}   