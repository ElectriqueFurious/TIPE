#include "out.h"
#include <stdio.h>
#include <stdlib.h>

char* convert_to_string(int value){
    switch (value)
    {
    case 0 :
        return " ";
        break;
    case 1 :
        return "1";
        break;
    case 2 :
        return "x";
        break;
    case 3 :
        return "3";
        break;
    case 4 :
        return "4";
        break;
    case 5 :
        return "5";
        break;
    case 6 :
        return "6";
        break;
    default:
        return "";
        break;
    }
}

void display_grid(int** grid, int height, int width, FILE* file){
    for(int i = 0;i<height;i++){
        fprintf(file,"| ");
        for(int j=0;j<width;j++){
            fprintf(file,"%s ",convert_to_string(grid[i][j]));
        }
        fprintf(file,"|\n");
    }
    fprintf(file,"\n");
}

void display_data(int** grid, int height, int width,FILE* file){
    fprintf(file,"O\n");
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            if(grid[i][j]!=0){
                fprintf(file,"%d;%d;%d\n",i,j,grid[i][j]);
            }
        }
    }
    fprintf(file,"C\n");
}

void display_scene(scene_object* scene,FILE* file){
    if(file == stdout){
        fprintf(file,"Scene in: %d by %d; turn number: %d\n\n",
            scene->width,scene->height,scene->time);
        display_grid(scene->grid,scene->height,scene->width,file);
    }
    else{
        display_data(scene->grid,scene->height,scene->width,file);
    }
    
}