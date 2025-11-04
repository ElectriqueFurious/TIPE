#ifndef _THREADS_
#define _THREADS_

#include "stack.h"
#include "priority.h"
#include "../scene/scene.h"
#include "../entity/player.h"
#include <pthread.h>

typedef struct {
    scene_object** future;
    int time;
    int accuracy;
    position* end;
    stack* closedList;
    priority_list* openList;
    pthread_mutex_t* m_prio;
    pthread_mutex_t* m_stack;
} A_arg;

void* multicore_A_star(void*);

#endif