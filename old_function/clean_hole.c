#include "../entity/position.h"
#include "../scene/scene.h"

void clean_hole(scene_object* scene,int color){
    for(int i=0;i<scene->height;i++){
        for(int j=0;j<scene->width;j++){
            if(scene->grid[i][j]!=color){
                if(i==0){
                    if(j==0){
                        if(scene->grid[i][j+1]==color && scene->grid[i+1][j]==color) scene->grid[i][j] = color;
                    }
                    else if(j==scene->width-1){
                        if(scene->grid[i][j-1]==color && scene->grid[i+1][j]==color) scene->grid[i][j] = color;
                    }
                    else{
                        if(scene->grid[i][j-1]==color && scene->grid[i][j+1]==color && scene->grid[i+1][j]==color) scene->grid[i][j] = color;
                    }
                }
                else if(i==scene->height-1){
                    if(j==0){
                        if(scene->grid[i-1][j]==color && scene->grid[i][j+1]==color) scene->grid[i][j] = color;
                    }
                    else if(j==scene->width-1){
                        if(scene->grid[i-1][j]==color && scene->grid[i][j-1]==color) scene->grid[i][j] = color;
                    }
                    else{
                        if(scene->grid[i-1][j]==color && scene->grid[i][j-1]==color && scene->grid[i][j+1]==color) scene->grid[i][j] = color;
                    }
                }
                else{
                    if(j==0){
                        if(scene->grid[i-1][j]==color && scene->grid[i][j+1]==color && scene->grid[i+1][j]==color) scene->grid[i][j] = color;
                    }
                    else if(j==scene->width-1){
                        if(scene->grid[i-1][j]==color && scene->grid[i][j-1]==color && scene->grid[i+1][j]==color) scene->grid[i][j] = color;
                    }
                    else{
                        if(scene->grid[i-1][j]==color && scene->grid[i][j-1]==color && scene->grid[i][j+1]==color && scene->grid[i+1][j]==color) scene->grid[i][j] = color;
                    }
                }
            }
        }
    }
}