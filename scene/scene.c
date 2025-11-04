#include <stdio.h>
#include <stdlib.h>
#include "scene.h"
#include "../tools/out.h"

scene_object* new_scene(int width, int height, int init){
    scene_object* scene = malloc(sizeof(scene_object));
    int** grid = malloc(height*sizeof(int*));
    for(int i=0;i<height;i++){
        int* row = calloc(width,sizeof(int));
        grid[i] = row;
    }
    scene->height = height;
    scene->width = width;
    scene->grid = grid;
    scene->time = 0;
    scene->init = init;
    return scene;
}

void scene_init(scene_object* scene,int value){
    for(int i=0;i<scene->height;i++){
        for(int j=0;j<scene->width;j++){
            scene->grid[i][j] = value;
        }
    }
}

void delete_scene(scene_object* scene){
    for(int i=0; i< scene->height;i++){
        free(scene->grid[i]);
    }
    free(scene->grid);
    free(scene);
}

void wait(scene_object* scene){
    scene->time++;
}

scene_object* copy_scene(scene_object* scene){
    scene_object* res = new_scene(scene->width,scene->height,scene->init);
    for(int i=0; i<res->height;i++){
        for(int j=0; j<res->width;j++){
            res->grid[i][j] = scene->grid[i][j];
        }
    }
    res->init=scene->init;
    return res;
}

