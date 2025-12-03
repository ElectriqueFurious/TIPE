#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include "values.h"
#include "entity/guard.h"
#include "entity/player.h"
#include "scene/scene.h"
#include "scene/simulation.h"

#define SOURCE_NAME_1 "./out/score/"
#define SOURCE_NAME_2 "./out/rotation/"
#define EXT ".txt"
#define MAX_BEHAVE 4

char* filled_zero(int k){
    char* res;
    if(k>=0){
        res = malloc((k+1)*sizeof(char));
        for(int i=0; i<k; i++){
            res[i] = '0';
        }
        res[k] = '\0';
    }
    else{
        res = malloc(sizeof(char));
        res[0] = '\0';
    }
    return res;
}

int nb_zeros(int k){
    if(k==0){
        return 1;
    }
    int cur = k;
    int res = 0;
    while(cur != 0){
        cur = cur/10;
        res++;
    }
    return res;
}

char* new_name(int behave, int number, char* source_name){
    char name[20];
    char* postname = malloc(30*sizeof(char));
    char* zero1 = filled_zero(2-nb_zeros(behave));
    char* zero2 = filled_zero(3-nb_zeros(number));
    sprintf(name,"%s%d_%s%d",zero1,behave,zero2,number);
    sprintf(postname,"%s",source_name);
    char* filename = strcat(name,EXT);
    char* file = strcat(postname,filename);
    free(zero1);
    free(zero2);
    return file;
}

guard_list** copy_lab(guard_list** storage, int size, int behave, int r_speed){
    guard_list** lab = malloc(size*sizeof(guard_list*));
    for(int i=0; i<size; i++){
        lab[i] = copy_guard_list(storage[i],behave,r_speed);
    }
    return lab;
}

void delete_lab(guard_list** lab, int size){
    for(int i=0; i<size; i++){
        delete_guard_list(lab[i]);
    }
    free(lab);
}

typedef struct {
    int k;
    int number;
    bool behave;
    int behave_angle;
    int accuracy;
    guard_list** storage;
} k_sim_arg;

void* thread_simulation(void* parameter){
    k_sim_arg* args = parameter; 
    guard_list** lab;
    char* file;
    if(args->behave){
        lab = copy_lab(args->storage,args->k,args->behave_angle,ROTATION_SPEED);
        file = new_name(args->behave_angle,args->number,SOURCE_NAME_1);
    }
    else{
        lab = copy_lab(args->storage,args->k,6,args->behave_angle);//Change behavior
        file = new_name(args->behave_angle,args->number,SOURCE_NAME_2);
    }
    FILE* score = fopen(file,"w");//to preserve the previous ones

    for(int i=0; i<args->k; i++){
        scene_object* scene = new_scene(WIDTH,HEIGHT,INIT);
        player* bot = new_player(WIDTH/2,0);
        if(args->behave_angle == 0){
            fprintf(score,"%d\n",simulate(scene,lab[i],bot,2,args->accuracy,false,NULL));
        }
        else{
            fprintf(score,"%d\n",simulate(scene,lab[i],bot,5,args->accuracy,false,NULL));
        }
        delete_scene(scene);
        delete_player(bot);
    }
    delete_lab(lab,args->k);
    fclose(score);
    free(file);
    pthread_exit(NULL);
}

void k_simulation(int k,int number,int behave_start,int behave_end, int accuracy){
    printf("number: %d\n",number);
    guard_list** storage = malloc(k*sizeof(guard_list*));
    for(int i=0; i<k; i++){
        storage[i] = new_guard_list(number,0,ROTATION_SPEED,1,4);
    }
    pthread_t* core = malloc((behave_end-behave_start+1)*sizeof(pthread_t));
    k_sim_arg* parametres = malloc((behave_end-behave_start+1)*sizeof(k_sim_arg));

    for(int behave=behave_start; behave<=behave_end; behave++){
        k_sim_arg parametre = {k,number,true,behave,accuracy,storage};
        parametres[behave-behave_start] = parametre;
        pthread_create(&core[behave-behave_start],NULL
            ,thread_simulation,&parametres[behave-behave_start]);
    }
    for(int behave=behave_start; behave<=behave_end; behave++){
        pthread_join(core[behave-behave_start],NULL);
    }

    free(core);
    free(parametres);
    delete_lab(storage,k);
}

void simulate_from_to(int k, int n, int m, int step, int range, int accuracy){
    for(int i=n; i<=m; i+= step){
        k_simulation(k,i,5,6,accuracy); //Select the Range
    }
}

void k_simulation_rotation(int k,int number,int angle_start,int angle_end, int step, int accuracy){
    printf("number: %d\n",number);
    guard_list** storage = malloc(k*sizeof(guard_list*));
    for(int i=0; i<k; i++){
        storage[i] = new_guard_list(number,0,ROTATION_SPEED,1,4);
    }
    pthread_t* core = malloc((angle_end-angle_start+1)*sizeof(pthread_t));
    k_sim_arg* parametres = malloc((angle_end-angle_start+1)*sizeof(k_sim_arg));

    for(int r_s=angle_start; r_s<=angle_end; r_s+=step){
        k_sim_arg parametre = {k,number,false,r_s,accuracy,storage};
        parametres[r_s-angle_start] = parametre;
        pthread_create(&core[r_s-angle_start],NULL,thread_simulation,&parametres[r_s-angle_start]);
    }
    for(int r_s=angle_start; r_s<=angle_end; r_s+=step){
        pthread_join(core[r_s-angle_start],NULL);
    }

    free(core);
    free(parametres);
    delete_lab(storage,k);
}

void find_angle_rotation(int angle_start, int angle_end, int angle_step, int k, 
                            int n, int m, int step, int accuracy){
    for(int i=n; i<=m; i+=step){
        k_simulation_rotation(k,i,angle_start,angle_end,angle_step,accuracy);
    }
}

int main(int argc, char **argv){
    int seed = time(NULL);
    // seed = 1716554888;
    srand(seed);
    printf("Seed: %d\n",seed);

    if(argc < 6){
        printf("\nWARNING!!!\n \
        \nNeed 5 parameters (in this order) :\
        \n\t-k : the number of simulation per situation\
        \n\t-n and m : simulate from n to m guards\
        \n\t-step : it's the step\
        \n\t-accuracy : accuracy of the player\n\n");
    }
    else{
        int k = atoi(argv[1]);
        int n = atoi(argv[2]);
        int m = atoi(argv[3]);
        int step = atoi(argv[4]);
        int accuracy = atoi(argv[5]);
        simulate_from_to(k,n,m,step,MAX_BEHAVE,accuracy);
        // find_angle_rotation(0,20,2,k,n,m,step,accuracy);
    }

    return 0;
}