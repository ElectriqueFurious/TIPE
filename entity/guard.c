#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "player.h"
#include "guard.h"
#include "../tools/draw.h"
#include "../values.h"

guard* new_guard(int angle, int behave, int rotation_speed, position* pos){
    guard* res = calloc(1,sizeof(guard));
    res->angle = angle;
    res->behave = behave;
    res->rotation_speed = rotation_speed;
    res->pos = pos;
    res->to_do.angle=0;
    res->to_do.there = *pos;
    initializer(res);
    return res;
}

void delete_guard(guard* guard){
    delete_position(guard->pos);
    free(guard->option.position_list);
    free(guard->option.angle_list);
    free(guard);
}

guard_list* new_guard_list(int nb, int behave,int r_speed ,int color, int color2){
    guard_list* res = malloc(sizeof(guard_list));
    res->nb = nb;
    res->color = color;
    res->color2 = color2;
    res->tab = malloc(nb*sizeof(guard*));
    for(int i=0; i<nb; i++){
        position* pos;
        int x = rand()%100;
        int y = rand()%100;
        if(x<=WIDTH/2+VISION_DISTANCE && x>=WIDTH/2-VISION_DISTANCE 
           && y<=VISION_DISTANCE)
            i--;
        else{

            pos = new_position(x,y);
            res->tab[i] = new_guard((360 + rand()%360)%360,behave,r_speed,pos);
        }
    }
    return res;
}

void delete_guard_list(guard_list* guard_list){
    for(int i=0; i<guard_list->nb; i++){
        delete_guard(guard_list->tab[i]);
    }
    free(guard_list->tab);
    free(guard_list);
}

guard* copy_guard(guard* guard, int behave, int r_speed){
    position* pos = copy_position(guard->pos);
    return new_guard(guard->angle,behave,r_speed,pos);
}

guard_list* copy_guard_list(guard_list* guards, int behave, int r_speed){
    guard_list* res = malloc(sizeof(guard_list));
    res->nb = guards->nb;
    res->color = guards->color;
    res->color2 = guards->color2;
    res->tab = malloc(res->nb*sizeof(guard*));
    for(int i=0; i<res->nb; i++){
        res->tab[i] = copy_guard(guards->tab[i],behave,r_speed);
    }
    return res;
}

void edit_behaves(guard_list* guards, int behave){
    for(int i=0; i<guards->nb; i++){
        guards->tab[i]->behave = behave;
        free(guards->tab[i]->option.angle_list);
        free(guards->tab[i]->option.position_list);
        initializer(guards->tab[i]);
    }
}

void behave(guard_list* guards, scene_object* scene, int time){
    scene_init(scene,scene->init);
    int* leader = find_leaders(guards);
    if(time == 0){
        arange_gards(guards);
    }
    for(int i=0;i<guards->nb;i++){
        guard* guard = guards->tab[i];
        guard->option.move_function(guards,scene,i,leader[i]);
        scene->time = time;
        draw_cone_with_cross(guard->pos,VISION_DISTANCE,
            guard->angle,VISION_FIELD,scene,guards->color,guards->color2);
        clean_holl(scene,guards->color,guards->color2);
    }
    // free(leader);
}

void guard_rotate_by(guard* guard, int deg){
    goal* target = &guard->to_do;
    if(target->angle < guard->rotation_speed){
        target->angle = deg;
    }

    int signe = target->angle > 0 ? 1 : -1;
    if(target->angle*signe > guard->rotation_speed){
        target->angle += (-guard->rotation_speed)*signe;
        guard->angle += guard->rotation_speed*signe;
        target->rotating = true;
    }
    else{
        guard->angle += target->angle;
        target->angle = 0;
        target->rotating = false;
    }
    guard->angle = (360+guard->angle)%360;
}

void guard_rotate_to(guard* guard, int angle){
    int deg = abs(angle)-guard->angle;
    int signe = deg>0 ? 1 : -1;
    if(angle>=0){
        guard_rotate_by(guard,deg*signe>180 ? (deg*signe-360)*signe : deg);
    }
    else{
        guard_rotate_by(guard,deg*signe>180 ? deg : (deg*signe-360)*signe);
    }
}

void guard_move_to(guard* guard, position pos, scene_object* scene){
    goal* target = &guard->to_do;
        position* p1 = guard->pos;
        position* p2 = &target->there;
        int err2 = 2 * target->err;
        if (err2 + target->dy > 0) {
            target->err -= target->dy;
            p1->x += (p1->x - p2->x < 0) ? 1 : -1;
        }
        if (0 < target->dx - err2) {
            target->err += target->dx;
            p1->y += (p1->y - p2->y < 0) ? 1 : -1;
        }
        if(equals(p1,p2)){
            target->there = pos;
            check_position(&target->there,scene);
            target->dx = abs(p2->x - p1->x);
            target->dy = abs(p2->y - p1->y);
            target->err = target->dx - target->dy;
            target->moving = false;
        }
}

void guard_natural_move_to(guard* guard, position pos, scene_object* scene){
    goal* target = &guard->to_do;
    int x = pos.x-guard->pos->x;
    int y = pos.y-guard->pos->y;
    int angle = guard->angle;
    if(x!=0 || y!=0){
        angle = ((int)(-atan2(y,x)*(180.0)/(3.14)) + 360)%360;
    }    
    if(!target->moving){
        guard_rotate_to(guard,angle);
    }
    if(!target->rotating || target->moving){
        target->moving = true;
        guard_move_to(guard,pos,scene);
    }
}

void _update_classe(guard_list* guards,int* res, int* classe, bool last){
    for(int i=0; i<guards->nb;i++){
        float min_range = range(guards->tab[i]->pos,guards->tab[res[0]]->pos); 
        for(int j=1; j<NB_LEADERS;j++){
            float r = range(guards->tab[i]->pos,guards->tab[res[j]]->pos);
            if(r<min_range){
                min_range = r;
                classe[i] = (last?res[j]:j);
            }
        }
    }
}

void _closest_guards(guard_list* guards, int* res, position* tmp){
    for(int j=0; j<NB_LEADERS;j++){
        float min_range = range(guards->tab[0]->pos,&tmp[j]);
        for(int i=1; i<guards->nb; i++){
            float r = range(guards->tab[i]->pos,&tmp[j]);
            if(r<min_range){
                min_range = r;
                res[j] = i;
            }
        }
    }
}

void _update_res(guard_list* guards, int* res, int* classe){
    position* tmp = calloc(NB_LEADERS,sizeof(position));
    int* count = calloc(NB_LEADERS,sizeof(int));
    for(int i=0;i<guards->nb;i++){
        tmp[classe[i]].x += guards->tab[i]->pos->x;
        tmp[classe[i]].y += guards->tab[i]->pos->y;
        count[classe[i]] ++;
    }
    for(int i=0;i<NB_LEADERS;i++){
        int n = (count[i]>0?count[i]:1);
        tmp[i].x = tmp[i].x/n;
        tmp[i].y = tmp[i].y/n;
    }
    free(count);
    _closest_guards(guards,res,tmp);
    free(tmp);
}

int* find_leaders(guard_list* guards){
    int* res = malloc(NB_LEADERS*sizeof(int));
    int* classe = calloc(guards->nb,sizeof(int));
    for(int i=0;i<NB_LEADERS;i++){
        res[i] = i%guards->nb;
    }
    for(int i=0;i<K_MEAN_ACCURACY;i++){
        _update_classe(guards,res,classe,false);
        _update_res(guards,res,classe);
    }
    _update_classe(guards,res,classe,true);
    free(res);
    return classe;
}

void _guards_colone(guard_list* guards, int* res, position* tmp){
    for(int i=0; i<guards->nb; i++){
        float min_range = range(guards->tab[i]->pos,&tmp[0]);
        for(int j=1; j<guards->nb;j++){
            float r = range(guards->tab[i]->pos,&tmp[j]);
            if(r<min_range){
                min_range = r;
                res[i] = j;
            }
        }
    }
}

void _swap_guard(guard** tab, int i, int j){
    guard* tmp = tab[i];
    tab[i] = tab[j];
    tab[j] = tmp;
}

void arange_gards(guard_list* guards){
    int size = guards->nb;
    int* colone = calloc(size,sizeof(int));
    position* tmp = malloc(size*sizeof(position));
    bool* is_use = calloc(size,sizeof(bool));
    for(int i=0; i<size; i++){
        tmp[i].x = (int)(i*(double)WIDTH/size);
        tmp[i].y = HEIGHT/2;
    }
    _guards_colone(guards,colone,tmp);
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            int k = (colone[i]+j)%size;
            if(!is_use[k]){
                is_use[k] = true;
                _swap_guard(guards->tab,i,k);
                break;
            }
        }
    }
    free(tmp);
    free(colone);
    free(is_use);
}

//____________________BEHAVES____________________

void initializer(guard* res){
    void (*init)(guard*);
    switch (res->behave){
            case 1:
                init = &rotation;
                break;
            case 2:
                init = &random_line;
                break;
            case 3:
                init = &corridor;
                break;
            case 4:
                init = &centred_square;
                break;
            case 5:
                init = &mousaid;
                break;
            case 6:
                init = &snake;
                break;
            default:
                init = &nothing;
                break;
        }
    init(res);
}

void nothing(guard* guard){
    guard->option.angle_list = calloc(1,sizeof(int));
    guard->option.position_list = malloc(sizeof(position));
    guard->option.position_list[0] = *guard->pos;
    guard->option.list_size = 1;
    guard->option.move_function = &classic_move_fonction;
}

void rotation(guard* guard){
    nothing(guard);
    guard->option.angle_list[0] = 360;
    guard->option.move_function = &speed_rotation_fonction;
}

void random_line(guard* guard){
    nothing(guard);
    guard->option.move_function = &random_move_fonction;
}

void corridor(guard* guard){
    guard->option.angle_list = calloc(2,sizeof(int));
    guard->option.position_list = malloc(2*sizeof(position));
    guard->option.position_list[0] = *guard->pos;
    guard->option.position_list[1] = *guard->pos;
    switch ((guard->angle-45)/90){
    case 0:
        guard->option.position_list[1].x += 20;
        break;
    case 1:
        guard->option.position_list[1].y -= 20;
        break;
    case 2:
        guard->option.position_list[1].x -= 20;
        break;
    default:
        guard->option.position_list[1].y += 20;
        break;
    }
    scene_object* scene = new_scene(WIDTH,HEIGHT,0);
    check_position(&guard->option.position_list[1],scene);
    delete_scene(scene);
    guard->option.list_size = 2;
    guard->option.move_function = &classic_move_fonction;
}

int _min_dist(guard* guard){
    int res = 0;
    position* list = guard->option.position_list;
    int size = guard->option.list_size;
    for(int i=1;i<size;i++){
        if(range(guard->pos,&list[i]) < range(guard->pos,&list[res])){
            res = i;
        }
    }
    if((guard->pos->x == list[res].x && guard->pos->x == list[(res+1)%size].x)
        || (guard->pos->y == list[res].y && guard->pos->y == list[(res+1)%size].y)){
        for(int i=0; i<size/2; i++){
            position tmp = list[i];
            list[i] = list[size-1-i];
            list[size-1-i] = tmp;
        }
        res = size-1-res;
    }
    return res;
}

void centred_square(guard* guard){
    int mid_x = WIDTH/2;
    int mid_y = HEIGHT/2;
    int r_x = abs(guard->pos->x - mid_x);
    int r_y = abs(guard->pos->y - mid_y);
    if(r_x<VISION_DISTANCE && r_y<VISION_DISTANCE){
        rotation(guard);
    }
    else{
        guard->option.list_size = 4;
        guard->option.angle_list = calloc(4,sizeof(int));
        guard->option.position_list = malloc(4*sizeof(position));
        int r = fmax(r_x,r_y);
        for(int i=0;i<4;i++){
            int s[2] = {1,-1};
            position p = {mid_x + s[i/2]*r, mid_y + s[(i+(i>1?1:0))%2]*r};
            guard->option.position_list[i]=p;
        }
        guard->option.current_task = _min_dist(guard);
        guard->option.move_function = &classic_move_fonction;
    }
}

void mousaid(guard* guard){
    guard->option.angle_list = calloc(1,sizeof(int));
    guard->option.position_list = malloc(1*sizeof(position));
    guard->option.list_size = 1;
    guard->option.position_list[0] = *guard->pos;
    guard->option.current_task = 0;
    guard->option.move_function = &mousaid_move_fonction;
}

void snake(guard* guard){
    guard->option.angle_list = calloc(5,sizeof(int));
    guard->option.position_list = malloc(5*sizeof(position));
    guard->option.list_size = 5;
    for(int i=0; i<5; i++){
        guard->option.position_list[i] = *guard->pos;
    }
    guard->option.current_task = 0;
    guard->option.move_function = &snake_move_fonction;
}

void speed_rotation_fonction(guard_list* guards, scene_object* scene, int i, int leader){
    guard* guard = guards->tab[i];
    guard->angle = (guard->angle + guard->rotation_speed)%360;
}

void classic_move_fonction(guard_list* guards, scene_object* scene, int i, int leader){
    guard* guard = guards->tab[i];
    behave_option option = guard->option;
    int current = option.current_task;
    position next_pos = option.position_list[current];
    if(option.angle_list[current] != 0){
        guard_rotate_by(guard,option.angle_list[current]);
    }
    else{
        guard_natural_move_to(guard,next_pos,scene);
    }
    if(option.angle_list[current] == 0 && equals(guard->pos,&next_pos)){
        guard->option.current_task = (current+1)%option.list_size;
    }
}

void random_move_fonction(guard_list* guards, scene_object* scene, int i, int leader){
    guard* guard = guards->tab[i];
    behave_option option = guard->option;
    position next_pos = option.position_list[0];
    guard_natural_move_to(guard,next_pos,scene);
    if(equals(guard->pos,&next_pos)){
        guard->option.position_list[0].x = rand()%scene->width;
        guard->option.position_list[0].y = rand()%scene->height;
    }
}

void _bounce_position(position* pos, scene_object* scene){
    int b1 = pos->x/scene->width > 0;
    int b2 = pos->y/scene->height > 0;
    if(pos->x < 0 || b1){
        pos->x = -pos->x + 2*b1*scene->width;
    }
    if(pos->y < 0 || b2){
        pos->y = -pos->y + 2*b2*scene->height;
    }
    check_position(pos,scene);
}

void _micro_decision(guard* leader,scene_object* scene, guard* guard){
    int D = 8;
    int x_min=guard->pos->x-D;
    int x_max=guard->pos->x+D;
    int y_min=guard->pos->y-D;
    int y_max=guard->pos->y+D;
    position* p1 = guard->pos;
    position* p = &guard->option.position_list[0];
    for(int x=x_min;x<=x_max ;x++){
        for(int y=y_min;y<=y_max ;y++){
            position q = {x,y};
            check_position(&q,scene);
            if(scene->grid[q.y][q.x] != scene->init){
                    p->y += (p1->y-q.y>0?1:-1);
                    p->x += (p1->x-q.x>0?1:-1);
            }
        }
    }
}

void mousaid_move_fonction(guard_list* guards, scene_object* scene, int i, int j){
    guard* leader = guards->tab[j];
    guard* guard = guards->tab[i];
    behave_option option = guard->option;
    position next_pos = option.position_list[0];
    int D = 3;
    if(j == i){
        guard_natural_move_to(guard,next_pos,scene);
        if(equals(guard->pos,&next_pos)){
            position new_pos = *guard->pos;
            int angle = guard->angle*(3.14/180);
            new_pos.x += (int)(cos(angle)*D);
            new_pos.y -= (int)(sin(angle)*D);
            _bounce_position(&new_pos,scene);
            option.position_list[0] = new_pos;
        }
    }
    else{
        guard_natural_move_to(guard,next_pos,scene);
            position* p1 = guard->pos;
            position* p2 = leader->pos;
            position p = leader->option.position_list[0];
            if(range(p1,p2)<VISION_DISTANCE){
                if(((leader->angle-45)/90)%2){
                    p.y += (p1->y-p2->y>0?1:-1)*VISION_DISTANCE;
                }
                else{
                    p.x += (p1->x-p2->x>0?1:-1)*VISION_DISTANCE;
                }
            }
            guard->option.position_list[0]=p;
            _micro_decision(leader,scene,guard);
            _bounce_position(&option.position_list[0],scene);

    }
}

void snake_move_fonction(guard_list* guards, scene_object* scene, int i, int leader){
    guard* guard = guards->tab[i];
    behave_option option = guard->option;
    int current = option.current_task;
    position next_pos = option.position_list[current];
    guard_natural_move_to(guard,next_pos,scene);
    if(option.angle_list[current] == 0 && equals(guard->pos,&next_pos)){
        guard->option.current_task = (current+1)%option.list_size;
        if(guard->option.current_task == 0){
            int x1 = (int)(i*(double)scene->width/guards->nb);
            int x2 = (int)((i+1)*(double)scene->width/guards->nb);
            if(x2>=scene->width){
                x2 = scene->width-1;
            }
            int dy = sin(VISION_FIELD*3.14/360)*VISION_DISTANCE;
            guard->option.current_task++;
            int x[2] = {x2,x1};
            int h = scene->height;
            for(int j=1;j<5;j++){
                guard->option.position_list[j].y = (guard->pos->y-(j/2)*dy+h)%h;
                guard->option.position_list[j].x = x[(j-1)/2];
            }
        }
    }
}