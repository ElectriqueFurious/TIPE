void k_simulation(int k,int number,int behave_start,int behave_end, int accuracy){
    guard_list** storage = malloc(k*sizeof(guard_list*));
    for(int i=0; i<k; i++){
        storage[i] = new_guard_list(number,0,ROTATION_SPEED,1,4);
    }

    for(int behave=behave_start; behave<=behave_end; behave++){
        guard_list** lab = copy_lab(storage,k,behave);
        char* file = new_name(behave,number);
        FILE* score = fopen(file,"w");

        for(int i=0; i<k; i++){
            scene_object* scene = new_scene(WIDTH,HEIGHT,INIT);
            player* bot = new_player(WIDTH/2,0);
            if(behave == 0){
                fprintf(score,"%d\n",simulate(scene,lab[i],bot,3,accuracy,false,NULL));
            }
            else{
                fprintf(score,"%d\n",simulate(scene,lab[i],bot,10,accuracy,false,NULL));
            }
            //multithreads there

            delete_scene(scene);
            delete_player(bot);
        }
        delete_lab(lab,k);
        fclose(score);
        free(file);
    }
    delete_lab(storage,k);
}