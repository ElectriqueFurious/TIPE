#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "../tools/graph.h"

player* new_player(int x, int y){
    player* bot = malloc(sizeof(player));
    position* pos = new_position(x, y);
    bot->pos = pos;
    return bot;
}

void delete_player(player* bot){
    delete_position(bot->pos);
    free(bot);
}

void move_left(position* pos,scene_object* scene){
    if(pos->x > 0){
        pos->x--;
    }
}

void move_right(position* pos,scene_object* scene){
    if(pos->x < scene->width-1){
        pos->x++;
    }
}

void move_down(position* pos,scene_object* scene){
    if(pos->y > 0){
        pos->y--;
    }
}

void move_up(position* pos,scene_object* scene){
    if(pos->y < scene->height-1){
        pos->y++;
    }
}

void naive_path(player* bot, scene_object** future) {
    scene_object* today = (future[0]->time > future[1]->time) ? future[1] : future[0];
    scene_object* tomorrow = (future[0]->time > future[1]->time) ? future[0] : future[1];

    if (bot->pos->x == 0) {
        if (bot->pos->y == 0) {
            if (check_and_move(bot, today, tomorrow, bot->pos->y + 1, bot->pos->x, move_up) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x + 1, move_right)) {
                return;
            }
        } else {
            if (check_and_move(bot, today, tomorrow, bot->pos->y + 1, bot->pos->x, move_up) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x + 1, move_right) ||
                check_and_move(bot, today, tomorrow, bot->pos->y - 1, bot->pos->x, move_down)) {
                return;
            }
        }
    } else if (bot->pos->x == today->width - 1) {
        if (bot->pos->y == 0) {
            if (check_and_move(bot, today, tomorrow, bot->pos->y + 1, bot->pos->x, move_up) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x - 1, move_left)) {
                return;
            }
        } else {
            if (check_and_move(bot, today, tomorrow, bot->pos->y + 1, bot->pos->x, move_up) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x - 1, move_left) ||
                check_and_move(bot, today, tomorrow, bot->pos->y - 1, bot->pos->x, move_down)) {
                return;
            }
        }
    } else {
        if (bot->pos->y == 0) {
            if (check_and_move(bot, today, tomorrow, bot->pos->y + 1, bot->pos->x, move_up) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x + 1, move_right) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x - 1, move_left)) {
                return;
            }
        } else {
            if (check_and_move(bot, today, tomorrow, bot->pos->y + 1, bot->pos->x, move_up) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x + 1, move_right) ||
                check_and_move(bot, today, tomorrow, bot->pos->y, bot->pos->x - 1, move_left) ||
                check_and_move(bot, today, tomorrow, bot->pos->y - 1, bot->pos->x, move_down)) {
                return;
            }
        }
    }
}

int check_and_move(player* bot, scene_object* today, scene_object* tomorrow,
                   int y, int x, void (*move_function)(position*, scene_object*)){
    if ((today->grid[y][x] == today->init) &&
        (tomorrow->grid[y][x] == tomorrow->init)) {
        move_function(bot->pos, today);
        return 1;
    }
    return 0;
}


void A_path(player* bot, scene_object** future, int time, int accuracy,stack* closedList, priority_list* openList){
    position* end = new_position(bot->pos->x,future[0]->height-1);
    position* next = A_star(future,time,accuracy,end,closedList,openList);
    if(next!=NULL){
        moveto(bot->pos,next->x,next->y);
    }
    delete_position(next);
    delete_position(end);
}

void path(player* bot, scene_object** future, int time,int accuracy,
          stack* closedList, priority_list* openList){
    if(accuracy == 2){
        naive_path(bot,future);
    }
    else{
        A_path(bot,future,time,accuracy,closedList,openList);
    }
}