#ifndef _GRAPH_
#define _GRAPH_

#include "../scene/scene.h"
#include "../entity/position.h"
#include "stack.h"
#include "priority.h"

list_position* neighbors2(scene_object*,position*,int,int);
list_position* neighbors(scene_object*,position*,int,int);
position* A_star(scene_object**,int,int,position*,stack*,priority_list*);

#endif