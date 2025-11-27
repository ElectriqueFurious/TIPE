#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tools/draw.h"
#include "../tools/out.h"
#include "simulation.h"
#include "../tools/graph.h"

int simulate(scene_object* scene, guard_list* guard_list, player* bot, int speed,
            int accuracy, bool export, FILE* output){
                
    int limit_time = scene->height*speed;
    int time = 0;

    scene_object** future = calloc(accuracy, sizeof(scene_object*));
    for(time=0; time<accuracy; time++){
        future[time] = copy_scene(scene);
        future[time]->time = time;
        behave(guard_list,future[time],time);
    }

    stack* closedList = new_stack();
    priority_list* openList = new_priority_list(1);
    position* start = copy_position(bot->pos);
    tree_node* rac = new_tree_node(*start,NULL,NULL);
    data* data_start = new_data(scene->height,start,rac,0);
    insert(data_start,openList);

    for(time = 0;time<limit_time;time++){
        draw_position(bot->pos,scene,2);
        // draw_position(guard_list->tab[0]->pos,scene,2);
        if(export){
            printf("Progress: %d%c; time = %d/%d\n"
                ,100*bot->pos->y/scene->height,'%',time,limit_time);
        }
        
        int curent_value = future[time%accuracy]->grid[bot->pos->y][bot->pos->x];
        if(curent_value == guard_list->color
            || curent_value == guard_list->color2){
            if(export){
                draw_position(bot->pos,future[time%accuracy],2);
                display_scene(future[time%accuracy],output);
            }
            time = -time;
            break;
        }
        if(bot->pos->y >= scene->height-1){
            break;
        }

        if(export){
            draw_position(bot->pos,future[time%accuracy],2);
            display_scene(future[time%accuracy],output);
        }

        behave(guard_list,future[(time+accuracy-1)%accuracy],time+accuracy-1);
        path(bot,future,time,accuracy,closedList,openList);
        //print path
        if(export){
            print_trajectory(future[(time+1)%accuracy],openList,5);
        }
        //end print path
        position end = {bot->pos->x,scene->height-1};
        adjust_priority_list(openList,bot->pos,end);
        adjust_stack(closedList,time);
    }

    for(int i=0; i<accuracy; i++){
        delete_scene(future[i]);
    }

    free(future);
    if(export){
        display_scene(scene,output);
    }
    delete_tree_node_rac(rac);
    delete_priority_list(openList);
    delete_stack(closedList);
    if(time == limit_time){
        time = -time;
    }
    return time;
}