#include <stdio.h>
#include <math.h>
#include "draw.h"

void check_position(position* pos, scene_object* scene){
    int b1 = pos->x/scene->width > 0;
    int b2 = pos->y/scene->height > 0;
    if(pos->x < 0 || b1){
        pos->x = b1*(scene->width-1);
    }
    if(pos->y < 0 || b2){
        pos->y = b2*(scene->height-1);
    }
}

void draw_position(position* pos, scene_object* scene, int color){
    check_position(pos,scene);
    scene->grid[pos->y][pos->x] = color;
}

void draw_cross(position* pos, scene_object* scene, int color){
    int x = pos->x;
    int y = pos->y;
    int all_x[5] = {x, x+1, x-1, x, x};
    int all_y[5] = {y, y, y, y+1, y-1};
    for(int i=0; i<5; i++){
        position* new_pos = new_position(all_x[i], all_y[i]);
        draw_position(new_pos,scene,color);
        delete_position(new_pos);
    }
}

list_position* select_circle(position* pos,int radius){
    int x = 0,y = radius, m = 5 - 4*radius;
    int cpt = 0; 
    while(x<=y){
        cpt++;
        if(m>0){
            y = y - 1;
            m = m - 8*y;
        }
        x = x + 1;
        m = m + 8*x + 4;
    }// count

    int size = cpt*8;
    list_position* res = new_list_position(size);
    x = 0,y = radius, m = 5 - 4*radius;

    for(int i = 0;i<cpt;i++){
		// res->tab[i] = new_position(y+pos->x, x+pos->y);// 0 to pi/4
        // res->tab[2*cpt - i - 1] = new_position(x+pos->x, y+pos->y);// pi/2 to pi/4

		// res->tab[i + 2*cpt] = new_position(-x+pos->x, y+pos->y);// pi/2 to 3pi/4
		// res->tab[4*cpt - i - 1] = new_position(-y+pos->x, x+pos->y);// pi to 3pi/4

		// res->tab[i + 4*cpt] = new_position(-y+pos->x, -x+pos->y);// pi to -3pi/4
		// res->tab[6*cpt - i - 1] = new_position(-x+pos->x, -y+pos->y);// -pi/2 to -3pi/4

		// res->tab[i + 6*cpt] = new_position(x+pos->x, -y+pos->y);// -pi/2 to -pi/4
		// res->tab[8*cpt - i - 1] = new_position(y+pos->x, -x+pos->y);// 0 to -pi/4

        for(int j =0; j<8;j++){
            int var1 = (j&1) ? (j+1)*cpt - i - 1 : j*cpt + i ;
            int var2_ = (!((j>>1)&1)^(j&1))*y + (((j>>1)&1)^(j&1))*x;
            int var2 = (j>1 && j<6) ? -var2_ : var2_ ;
            int var3_ = (!((j>>1)&1)^(j&1))*x + (((j>>1)&1)^(j&1))*y;
            int var3 = (j<4) ? var3_ : -var3_ ;
            res->tab[var1] = new_position(var2 + pos->x,var3 + pos->y);
        }

        if(m>0){
            y = y - 1;
            m = m - 8*y;
        }  
        x = x + 1;
        m = m + 8*x + 4;
    }

    res->size = size;

    return res;
}

list_position* select_arc(position* pos, int radius, int direction, int angle){
    list_position* circle = select_circle(pos, radius);
    direction = (360+direction%360)%360;
    int octant = circle->size/8;

    int start=0;
    float pourcentage = angle/720.0;
    int nb_points = circle->size  * pourcentage;
    
    if(direction !=0){
        int dire_45 = (direction-1)/45;
        start = (7 - dire_45)*octant;
        int condition1 = 45<direction && dire_45 == 1;
        int condition2 = 225<direction && dire_45 == 5;
        if(condition1 || condition2){
            start++;
        }
    }
 
    // if(325 < direction) start = 0; //switch
    // else if(270 < direction) start = octant;
    // else if(direction == 270) start = 2*octant;
    // else if(225 < direction) start = 2*octant+1;
    // else if(180 < direction) start = 3*octant;
    // else if(135 < direction) start = 4*octant;
    // else if(90 < direction) start = 5*octant;
    // else if(direction == 90) start = 6*octant;
    // else if(45 < direction) start = 6*octant+1;
    // else if(0 < direction) start = 7*octant;

    position* mid = circle->tab[start];
    int num = start;
    
    if(direction != 90 && direction != 270){
        double tan_direction = tan(direction*3.14/180);
        double tan_mid = (double)(pos->y - mid->y)/(pos->x - mid->x);
        for(int i = 0; i < octant;i++){
            double tan_point = (double)(pos->y - circle->tab[start+i]->y)/
                                (circle->tab[start+i]->x - pos->x);
            if(fabs(tan_mid - tan_direction) > fabs(tan_point - tan_direction)){ 
                tan_mid = tan_point;
                num = start + i;
                mid = circle->tab[num];
            }
        }
    }

    list_position* res = new_list_position(2*nb_points+1);
    res->tab[nb_points] = copy_position(mid);

    for(int i = 0; i<nb_points;i++){
        position* pos1 = circle->tab[(circle->size + num - i)%circle->size];
        position* pos2 = circle->tab[(i+num)%circle->size];
        res->tab[nb_points - i - 1] = copy_position(pos1); 
        res->tab[1 + i + nb_points] = copy_position(pos2);
    }

    delete_list_position(circle);

    return res;

}

void check_selection(list_position* arc, scene_object* scene) {
    for(int i = 0;i<arc->size;i++) {
        check_position(arc->tab[i],scene);
    }
}

list_position* delete_double(list_position* arc){
    int nb_doubles = 0;
    for(int i = 1; i <arc->size;i++){
        if (equals(arc->tab[i-1],arc->tab[i])){
            nb_doubles++;
        }
    }
    
    list_position* res = new_list_position(arc->size - nb_doubles);
    res->tab[0] = copy_position(arc->tab[0]);
    int nb_deleted = 0;

    for(int i = 1; i <arc->size;i++){
        if (!equals(arc->tab[i-1],arc->tab[i])){
            res->tab[i-nb_deleted] = copy_position(arc->tab[i]);
        }
        else{
            nb_deleted++;
        }
    }

    delete_list_position(arc);
    return res;
}

void _draw_line(position* p1, position* p2, scene_object* scene, 
               int color,int color2, bool cross){

    int dx = abs(p2->x - p1->x);
    int dy = abs(p2->y - p1->y);
    int incX = (p1->x < p2->x) ? 1 : -1;
    int incY = (p1->y < p2->y) ? 1 : -1;
    int err = dx - dy;
    position* p = new_position(p1->x,p1->y);
    draw_position(p, scene, color);
    do{
        if(!cross){
            draw_position(p, scene, color);
        }
        int err2 = 2 * err;
        if (err2 > -dy) {
            err -= dy;
            if(cross && err2 < dx){
                p->y += incY;
                draw_position(p, scene, color);
                p->y -= incY;
            }
            p->x += incX;
            if(cross){
                draw_position(p, scene, color);
            }
        }
        if (err2 < dx) {
            err += dx;
            p->y += incY;
            if(cross){
                draw_position(p, scene, color);
            }
        }
    } while (!equals(p,p2));
    draw_position(p, scene, color2);
    delete_position(p);
}

void draw_line(position* p1, position* p2, scene_object* scene, 
               int color,int color2){
    _draw_line(p1,p2,scene,color,color2,false);
}

void draw_line_in_cross(position* p1, position* p2, scene_object* scene, 
               int color,int color2){
    _draw_line(p1,p2,scene,color,color2,true);
}

void draw_cone(position* pos, int radius, int direction, int angle, 
               scene_object* scene, int color, int color2){

    list_position* arc = select_arc(pos,radius,direction,angle);
    check_selection(arc,scene);
    arc = delete_double(arc);
    draw_position(pos,scene,color);

    for(int i=0;i<arc->size;i++){
        draw_line(pos,arc->tab[i],scene,color,color2);
    }

    delete_list_position(arc);
}

void draw_cone_with_cross(position* pos, int radius, int direction, int angle, 
               scene_object* scene, int color, int color2){
    list_position* arc = select_arc(pos,radius,direction,angle);
    check_selection(arc,scene);
    arc = delete_double(arc);
    draw_position(pos,scene,color);

    draw_line(pos,arc->tab[0],scene,color,color2);
    draw_line(pos,arc->tab[arc->size-1],scene,color,color2);
    for(int i=1;i<arc->size-1;i++){
        draw_line_in_cross(pos,arc->tab[i],scene,color,color2);
    }

    delete_list_position(arc);
}

void quick_draw_cone(position* pos,int radius,int direction,int angle,
                     scene_object* scene,int color,int color2){
    list_position* arc = select_arc(pos,radius,direction,angle);
    check_selection(arc,scene);
    arc = delete_double(arc);

    draw_line(pos,arc->tab[0],scene,color,color2);
    draw_line(pos,arc->tab[arc->size-1],scene,color,color2);

    for(int i=0;i<arc->size;i++){
        draw_position(arc->tab[i],scene,color);
    }

    delete_list_position(arc);    
}

void print_trajectory(scene_object* scene, priority_list* list, int color){
    if(!empty_priority_list(list)){
        tree_node* elt = list->tas[1]->way;
        tree_node* prev = elt->prev;
        if(prev != NULL){
            while (prev->prev != NULL){
                position pos = {elt->pos.x,elt->pos.y};
                draw_position(&pos,scene,color);
                elt = prev;
                prev = elt->prev;
            }
        }
    }
}

void clean_holl(scene_object* scene, int color, int color2){
    for (int i = 0; i < scene->height; i++) {
        for (int j = 0; j < scene->width; j++) {
            if (scene->grid[i][j] == scene->init) {
                int neighbors = 0;
                if (i > 0 && 
                   (scene->grid[i - 1][j] == color 
                   || scene->grid[i - 1][j] == color2)) neighbors++;
                if (i < scene->height - 1 && 
                   (scene->grid[i + 1][j] == color 
                   || scene->grid[i + 1][j] == color2)) neighbors++;
                if (j > 0 && 
                   (scene->grid[i][j - 1] == color 
                   || scene->grid[i][j-1] == color2)) neighbors++;
                if (j < scene->width - 1 && 
                   (scene->grid[i][j + 1] == color 
                   || scene->grid[i][j + 1] == color2)) neighbors++;

                if (neighbors >= 3) scene->grid[i][j] = color;
            }
        }
    }
}
