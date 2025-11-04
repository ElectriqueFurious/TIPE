#ifndef _PLAYER_
#define _PLAYER_

#include "position.h"
#include "../tools/stack.h"
#include "../tools/priority.h"
#include "../scene/scene.h"

typedef struct {
    position* pos;
} player;

player* new_player(int,int);
void delete_player(player*);

void move_left(position*,scene_object*);
void move_right(position*,scene_object*);
void move_up(position*,scene_object*);
void move_down(position*,scene_object*);

void naive_path(player*,scene_object**);
void A_path(player*,scene_object**,int,int,stack*,priority_list*);

void multicore_path(player*,scene_object**,int,int,stack*,priority_list*,int);
void path(player*,scene_object**,int,int,stack*,priority_list*);
int check_and_move(player*,scene_object*,scene_object*,int,int,
                   void (*f)(position*, scene_object*));

#endif