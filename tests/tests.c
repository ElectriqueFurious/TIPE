#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "../values.h"
#include "../tools/draw.h"
#include "../scene/scene.h"
#include "../entity/player.h"
#include "../entity/guard.h"
#include "../scene/simulation.h"
#include "../tools/out.h"
#include "../entity/position.h"

void test_scene(){
    scene_object* scene = new_scene(10,5,0);
    scene_init(scene,scene->init);
    display_scene(scene,stdout);
    delete_scene(scene);
}

void test_player(){
    player* bot = new_player(0,0);
    delete_player(bot);
}

void test_guard(){
    guard_list* guards = new_guard_list(10,0,ROTATION_SPEED,1,4);
    delete_guard_list(guards);
}

void test_postion(){
    scene_object* scene = new_scene(10,5,0);
    position* pos = new_position(0,0);
    scene_init(scene,scene->init);

    draw_position(pos,scene,1);
    move_right(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    draw_position(pos,scene,2);
    move_up(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    move_down(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    move_left(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    draw_position(pos,scene,3);
    move_down(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    move_left(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    draw_position(pos,scene,4);
    display_scene(scene,stdout);

    moveto(pos,9,4);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    draw_position(pos,scene,5);
    move_right(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    draw_position(pos,scene,6);
    display_scene(scene,stdout);

    move_down(pos,scene);
    printf("position : (%d,%d)\n",pos->x,pos->y);
    draw_position(pos,scene,5);
    display_scene(scene,stdout);

    delete_position(pos);
    delete_scene(scene); 
}

void test_select_circle(){//pas bon à partir de la
    scene_object* scene = new_scene(101,101,0);
    scene_init(scene,scene->init);
    position* pos = new_position(50,50);
    list_position* circle = select_circle(pos,45);

    for(int i = 0; i < circle->size;i++){
        draw_position(circle->tab[i],scene,1);
        wait(scene);
        display_scene(scene,stdout);
    }

    printf("Il y a %d points !!\n\n",circle->size - 1);

    delete_position(pos);
    delete_list_position(circle);
    delete_scene(scene);
}

void test_select_arc(){
    scene_object* scene = new_scene(101,101,0);
    scene_init(scene,scene->init);
    position* pos = new_position(20,20);

    for(int j=0;j < 37;j++){
        list_position* arc = select_arc(pos,45,10*j,60);
        //modifier 360 pour voir la différence suivant les angels
        check_selection(arc,scene);
        arc = delete_double(arc);
        draw_position(pos,scene,0);
        for(int i=0; i<arc->size;i++){
            draw_position(arc->tab[i],scene,1);
        }
        draw_line(pos,arc->tab[arc->size/2],scene,1,4);
        wait(scene);
        display_scene(scene,stdout);
        scene_init(scene,scene->init);
        printf("Il y a %d points !!\n\n",arc->size - 1);
        delete_list_position(arc);
    }


    delete_position(pos);
    delete_scene(scene);
}

void test_draw_line(){
    scene_object* scene = new_scene(101,101,0);
    scene_init(scene,scene->init);
    position* pos = new_position(50,50);
    list_position* circle = delete_double(select_circle(pos,45));

    for(int i = 0; i< circle->size;i++){
        draw_line(pos,circle->tab[i],scene,1,4);
        display_scene(scene,stdout);
        scene_init(scene,scene->init);
        wait(scene);
    }

    delete_position(pos);
    delete_list_position(circle);
    delete_scene(scene);
}

void test_draw_cone(){
    scene_object* scene = new_scene(201,201,0);
    scene_init(scene,scene->init);
    position* pos = new_position(99,50);

    for(int i = 0; i< 37;i++){
        /*quick_*/draw_cone_with_cross(pos,100,10*i,60,scene,1,4);
        // clean_holl(scene,1,1);
        display_scene(scene,stdout);
        scene_init(scene,scene->init);
        wait(scene);
    }

    delete_position(pos);
    delete_scene(scene);
}

void test_copy_scene(){
    scene_object* scene1 = new_scene(10,10,0);
    scene_object* scene2 = copy_scene(scene1);

    scene_init(scene2, scene2->init);
    display_scene(scene1,stdout);
    display_scene(scene2,stdout);

    delete_scene(scene1);
    delete_scene(scene2);
}

void test_simulate_path(int n){
    int h = HEIGHT, w = WIDTH;
    scene_object* scene = new_scene(w,h,0);
    scene->init=0;
    scene_init(scene,scene->init);
    int number = 150;
    guard_list* guards = new_guard_list(number,3,ROTATION_SPEED,1,4);
    player* bot = new_player(w/2,0);

    // printf("init angle : %d\n",guards->tab[0]->angle);
    // position pos = {50,50};
    // moveto(guards->tab[0]->pos,50,50);

    FILE* f = fopen("./out/test.txt","w");
    fprintf(f,"%d\n%d\n",scene->width,scene->height);
    printf("Score : %d\n",simulate(scene,guards,bot,3,n,true,f));
    fclose(f);

    delete_guard_list(guards);
    delete_player(bot);
    delete_scene(scene);
}

int main(void){
    int seed = time(NULL);
    // seed = 1716130203;
    srand(seed);
    printf("seed : %d\n",seed);
    // test_scene();
    // test_player();
    // test_guard();
    // test_postion();
    // test_select_circle();
    // test_select_arc();
    //test_draw_line();
    // test_draw_cone();
    // test_copy_scene();
    test_simulate_path(100);

    return 0;
}