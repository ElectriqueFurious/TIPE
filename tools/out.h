#ifndef _OUT_
#define _OUT_

#include <stdio.h>
#include "../scene/scene.h"

char* convert_to_string(int);
void display_grid(int**,int,int,FILE*);
void display_scene(scene_object*,FILE*);

#endif