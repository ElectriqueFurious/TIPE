#ifndef _DRAW_
#define _DRAW_

#include "priority.h"
#include "../entity/position.h"
#include "../scene/scene.h"
#include <stdlib.h>

void check_position(position*,scene_object*);
void draw_position(position*,scene_object*,int);
void draw_cross(position*,scene_object*,int);

list_position* select_circle(position*,int);
list_position* select_arc(position*,int,int,int);
void check_selection(list_position*,scene_object*);
list_position* delete_double(list_position*);

void _draw_line(position*,position*,scene_object*,int,int,bool);
void draw_line(position*,position*,scene_object*,int,int);
void draw_line_in_cross(position*,position*,scene_object*,int,int);

void draw_cone(position*,int,int,int,scene_object*,int,int);
//colors the same cell multiple times but allows obstacles
void draw_cone_with_cross(position*,int,int,int,scene_object*,int,int);
void quick_draw_cone(position*,int,int,int,scene_object*,int,int);
void print_trajectory(scene_object*,priority_list*,int);

void clean_hole(scene_object*,int,int);

#endif