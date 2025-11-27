#include "../entity/player.h"
#include "../scene/scene.h"

void naive_path1(player* bot, scene_object** future){
    scene_object* today;
    scene_object* tomorrow;
    if(future[0]->time > future[1]->time){
        today = future[1];
        tomorrow = future[0];
    }
    else{
        today = future[0];
        tomorrow = future[1];
    }

    if(bot->pos->x == 0){// disjunction on x
        if(bot->pos->y == 0){// disjunction on y
            if((today->grid[bot->pos->y+1][bot->pos->x] == today->init) && (tomorrow->grid[bot->pos->y+1][bot->pos->x] == tomorrow->init)){
                move_up(bot->pos,today);
            }
            // else if((tomorrow->grid[bot->pos->y][bot->pos->x] == tomorrow->init)){
            //     moveto(bot->pos,bot->pos->x,bot->pos->y);// don't move
            // }
            else if((today->grid[bot->pos->y][bot->pos->x+1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x+1] == tomorrow->init)){
                move_right(bot->pos,today);
            }
        }
        else{
            if((today->grid[bot->pos->y+1][bot->pos->x] == today->init) && (tomorrow->grid[bot->pos->y+1][bot->pos->x] == tomorrow->init)){
                move_up(bot->pos,today);
            }
            // else if((tomorrow->grid[bot->pos->y][bot->pos->x] == tomorrow->init)){
            //     moveto(bot->pos,bot->pos->x,bot->pos->y);// don't move
            // }
            else if((today->grid[bot->pos->y][bot->pos->x+1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x+1] == tomorrow->init)){
                move_right(bot->pos,today);
            }
            else{
                move_down(bot->pos,today);
            }
        }
    }
    else if(bot->pos->x == today->width-1){
        if(bot->pos->y == 0){
            if((today->grid[bot->pos->y+1][bot->pos->x] == today->init) && (tomorrow->grid[bot->pos->y+1][bot->pos->x] == tomorrow->init)){
                move_up(bot->pos,today);
            }
            // else if((tomorrow->grid[bot->pos->y][bot->pos->x] == tomorrow->init)){
            //     moveto(bot->pos,bot->pos->x,bot->pos->y);// don't move
            // }
            else if((today->grid[bot->pos->y][bot->pos->x-1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x-1] == tomorrow->init)){
                move_left(bot->pos,today);
            }
        }
        else{
            if((today->grid[bot->pos->y+1][bot->pos->x] == today->init) && (tomorrow->grid[bot->pos->y+1][bot->pos->x] == tomorrow->init)){
                move_up(bot->pos,today);
            }
            // else if((tomorrow->grid[bot->pos->y][bot->pos->x] == tomorrow->init)){
            //     moveto(bot->pos,bot->pos->x,bot->pos->y);// don't move
            // }
            else if((today->grid[bot->pos->y][bot->pos->x-1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x-1] == tomorrow->init)){
                move_left(bot->pos,today);
            }
            else{
                move_down(bot->pos,today);
            }
        }
    }
    else{
        if(bot->pos->y == 0){
            if((today->grid[bot->pos->y+1][bot->pos->x] == today->init) && (tomorrow->grid[bot->pos->y+1][bot->pos->x] == tomorrow->init)){
                move_up(bot->pos,today);

            }
            // else if((tomorrow->grid[bot->pos->y][bot->pos->x] == tomorrow->init)){
            //     moveto(bot->pos,bot->pos->x,bot->pos->y);// don't move
            // }
            else if((today->grid[bot->pos->y][bot->pos->x+1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x+1] == tomorrow->init) && (today->grid[bot->pos->y][bot->pos->x-1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x-1] == tomorrow->init)){
                int left_range = abs(today->width/2 - bot->pos->x-1);
                int right_range = abs(today->width/2 - bot->pos->x+1);
                if(left_range < right_range){
                    move_left(bot->pos,today);
                }
                else{
                    move_right(bot->pos,today);
                }
            }
            else if((today->grid[bot->pos->y][bot->pos->x+1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x+1] == tomorrow->init)){
                move_right(bot->pos,today);
            }
            else if((today->grid[bot->pos->y][bot->pos->x-1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x-1] == tomorrow->init)){
                move_left(bot->pos,today);
            }
        }
        else{// Not close to the edges
            if((today->grid[bot->pos->y+1][bot->pos->x] == today->init) && (tomorrow->grid[bot->pos->y+1][bot->pos->x] == tomorrow->init)){
                move_up(bot->pos,today);

            }
            // else if((tomorrow->grid[bot->pos->y][bot->pos->x] == tomorrow->init)){
            //     moveto(bot->pos,bot->pos->x,bot->pos->y);// don't move
            // }
            else if((today->grid[bot->pos->y][bot->pos->x+1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x+1] == tomorrow->init) && (today->grid[bot->pos->y][bot->pos->x-1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x-1] == tomorrow->init)){
                
                int left_range = abs(today->width/2 - bot->pos->x-1);
                int right_range = abs(today->width/2 - bot->pos->x+1);
                if(left_range < right_range){
                    move_left(bot->pos,today);
                }
                else{
                    move_right(bot->pos,today);
                }
            }
            else if((today->grid[bot->pos->y][bot->pos->x+1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x+1] == tomorrow->init)){
                move_right(bot->pos,today);
            }
            else if((today->grid[bot->pos->y][bot->pos->x-1] == today->init) && (tomorrow->grid[bot->pos->y][bot->pos->x-1] == tomorrow->init)){
                move_left(bot->pos,today);
            }
            else{
                move_down(bot->pos,today);
            }
        }
    }
}