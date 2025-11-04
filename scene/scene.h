#ifndef _SCENE_
#define _SCENE_

typedef struct {
    int height;
    int width;
    int** grid;
    int init;
    int time;
} scene_object;

scene_object* new_scene(int,int,int);
void scene_init(scene_object*,int);
void delete_scene(scene_object*);

void wait(scene_object*);
scene_object* copy_scene(scene_object*);

#endif