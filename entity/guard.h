#ifndef _GUARD_
#define _GUARD_

#include "position.h"
#include "../scene/scene.h"

typedef struct{
    position there;
    int dx;
    int dy;
    int err;
    int angle;
    bool moving;
    bool rotating;
} goal; //Necessary for simple movements

typedef struct behave_option_t behave_option;
typedef struct guard_t guard;
typedef struct guard_list_t guard_list;

struct behave_option_t {
    int* angle_list;
    position* position_list;
    int list_size;
    int current_task;
    void (*move_function)(guard_list*,scene_object*,int,int);
};//Allows for complex behavior

struct guard_t{
    position* pos;
    int angle;
    int behave;
    int rotation_speed;
    goal to_do;
    behave_option option;
};

struct guard_list_t {
    int nb;
    int color;
    int color2;
    guard** tab;
};

guard* new_guard(int,int,int,position*);
void delete_guard(guard*);
guard_list* new_guard_list(int,int,int,int,int);
void delete_guard_list(guard_list*);

void initializer(guard*);

guard* copy_guard(guard*,int,int);
guard_list* copy_guard_list(guard_list*,int,int);
void edit_behaves(guard_list*,int);

void behave(guard_list*,scene_object*,int);

void guard_rotate_by(guard*,int);
void guard_rotate_to(guard*,int);
void guard_move_to(guard*,position,scene_object*);
void guard_natural_move_to(guard*,position,scene_object*);

int* find_leaders(guard_list*);
void arange_gards(guard_list*);

void nothing(guard*);
void rotation(guard*);
void random_line(guard*);
void corridor(guard*);
void centred_square(guard*);
void mousaid(guard*);
void snake(guard*);

void speed_rotation_fonction(guard_list*,scene_object*,int,int);
void classic_move_fonction(guard_list*,scene_object*,int,int);
void random_move_fonction(guard_list*,scene_object*,int,int);
void mousaid_move_fonction(guard_list*,scene_object*,int,int);
void snake_move_fonction(guard_list*,scene_object*,int,int);

#endif