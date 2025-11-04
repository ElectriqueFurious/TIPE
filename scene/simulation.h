#ifndef _SIMULATION_
#define _SIMULATION_

#include <stdbool.h>
#include <stdio.h>
#include "scene.h"
#include "../entity/player.h"
#include "../entity/guard.h"

int simulate(scene_object*,guard_list*,player*,int,int,bool,FILE*);

#endif