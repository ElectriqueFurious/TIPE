void multicore_path(player* bot, scene_object** future, int time, int accuracy, 
                    stack* closedList, priority_list* openList, int nb_threads){
    pthread_t* core = malloc(nb_threads*sizeof(pthread_t));
    pthread_mutex_t m_prio = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t m_stack = PTHREAD_MUTEX_INITIALIZER;
    
    position* end = new_position(bot->pos->x,future[0]->height-1);
    position* next = NULL;
    A_arg arg = {future,time,accuracy,end,closedList,openList,&m_prio,&m_stack};
    
    for(int i = 0; i < nb_threads; i++){
        pthread_create(&core[i],NULL,multicore_A_star,&arg);
    }
    for(int i = 0; i < nb_threads; i++){
        if(next != NULL){
            free(next);
        }
        pthread_join(core[i],(void*)&next);
    }
    
    if(next!=NULL){
        moveto(bot->pos,next->x,next->y);
    }
    free(core);
    delete_position(next);
    delete_position(end);
}
