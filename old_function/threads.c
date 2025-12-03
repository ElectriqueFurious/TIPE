#include <stdio.h>
#include "threads.h"
#include "graph.h"

void* multicore_A_star(void* args){
    scene_object** scene_list = ((A_arg*)args)->future;
    int time = ((A_arg*)args)->time;
    int accuracy = ((A_arg*)args)->accuracy;
    position* default_end = ((A_arg*)args)->end;
    stack* closedList = ((A_arg*)args)->closedList;
    priority_list* openList = ((A_arg*)args)->openList;
    pthread_mutex_t* m_prio = ((A_arg*)args)->m_prio;
    pthread_mutex_t* m_stack = ((A_arg*)args)->m_stack;

    position* res = NULL;
    
    int height = scene_list[0]->height;
    int width = scene_list[0]->width;
    position end = *default_end;

    while (true){
        pthread_mutex_lock(m_prio);
        data* u;
        bool is_empty = empty_priority_list(openList); 
        if(!is_empty){
            u = openList->heap[1];
            if(u->way != NULL && u->way->start != NULL){
                end.x = u->way->start->pos.x;
            }
            else{
                end.x = default_end->x;
            }

            if((u->node->x == end.x && u->node->y == end.y) || u->cost >= accuracy-1){
                position* res = copy_position(&(u->way->end->pos));
                pthread_mutex_unlock(m_prio);

                pthread_exit((void*)res);
        }
        
        u = remove_root(openList);
        }
        pthread_mutex_unlock(m_prio);

        if(!is_empty){
            list_position* neighbore = neighbors(scene_list[(time+u->cost+1)%accuracy],u->node,height,width);
            for(int i = 0; i<neighbore->size;i++){
                position* elt = neighbore->tab[i];
                
                pthread_mutex_lock(m_prio);
                bool condition = !(is_in_stack(elt,u->cost+1,closedList) || 
                    is_in_priority(openList,elt,u->cost+1));
                pthread_mutex_unlock(m_prio);

                if(condition){
                    elt = copy_position(elt);
                    int d = range(&end,elt);
                    queue_position* new_way = copy_queue(u->way);
                    enqueue(new_way,new_queue_elt(*elt));
                    data* v = new_data(u->cost+1+d,elt,new_way,u->cost+1);

                    pthread_mutex_lock(m_prio);
                    insert(v,openList);
                    pthread_mutex_unlock(m_prio);
                }
            }

            delete_list_position(neighbore);

            pthread_mutex_lock(m_stack);
            enstack(new_stack_elt(copy_position(u->node),u->cost),closedList);
            pthread_mutex_unlock(m_stack);

            delete_data(u);
        }
        else{
            break;
        }
    }

    pthread_exit((void*)res);
}