#include "../entity/player.h"

list_position* neighbors(scene_object* scene, position* pos, int height, int width){
    list_position* list = new_list_position(5);
    int cpt=0;
    
    if(pos->x == 0){
        if(pos->y == 0){
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x+1] == scene->init){
                list->tab[cpt] = new_position(pos->x+1,pos->y);
                cpt++;
            }
        }
        else if(pos->y == height-1){
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y-1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y-1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x+1] == scene->init){
                list->tab[cpt] = new_position(pos->x+1,pos->y);
                cpt++;
            }
        }
        else{
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y-1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y-1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x+1] == scene->init){
                list->tab[cpt] = new_position(pos->x+1,pos->y);
                cpt++;
            }
        }
    }
    else if(pos->x == width-1){
        if(pos->y == 0){
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x-1] == scene->init){
                list->tab[cpt] = new_position(pos->x-1,pos->y);
                cpt++;
            }
        }
        else if(pos->y == height-1){
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x-1] == scene->init){
                list->tab[cpt] = new_position(pos->x-1,pos->y);
                cpt++;
            }
        }
        else{
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y-1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y-1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x-1] == scene->init){
                list->tab[cpt] = new_position(pos->x-1,pos->y);
                cpt++;
            }
        }
    }
    else{
        if(pos->y == 0){

            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x+1] == scene->init){
                list->tab[cpt] = new_position(pos->x+1,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x-1] == scene->init){
                list->tab[cpt] = new_position(pos->x-1,pos->y);
                cpt++;
            }
        }
        else if(pos->y == height-1){
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y-1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y-1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x+1] == scene->init){
                list->tab[cpt] = new_position(pos->x+1,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x-1] == scene->init){
                list->tab[cpt] = new_position(pos->x-1,pos->y);
                cpt++;
            }
        }
        else{
            if(scene->grid[pos->y][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y+1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y+1);
                cpt++;
            }
            if(scene->grid[pos->y-1][pos->x] == scene->init){
                list->tab[cpt] = new_position(pos->x,pos->y-1);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x+1] == scene->init){
                list->tab[cpt] = new_position(pos->x+1,pos->y);
                cpt++;
            }
            if(scene->grid[pos->y][pos->x-1] == scene->init){
                list->tab[cpt] = new_position(pos->x-1,pos->y);
                cpt++;
            }
        }
    }
    list->size = cpt;
    return list;
}