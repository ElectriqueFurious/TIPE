#include "../entity/position.h"
#include "../scene/scene.h"
#include "../tools/draw.h"

void draw_line(position* p,position* p2,scene_object* scene,int color){

    position* p1 = copy_position(p);

    int dx = p2->x - p1->x;
    int dy = p2->y - p1->y;

    if (dx!=0){//Non vertical

        if(dx>0){//Partie droite

            if(dy!=0){//Non horizontal

                if(dy>0){//Partie haute, donc de 0 à pi/2

                    if(dx >= dy){//Donc 0 à pi/4, octant numéro 1

                        int e = dx;
                        dx = e*2;
                        dy = dy*2;

                        while(p1->x != p2->x){
                            draw_position(p1,scene, color);
                            e = e - dy;
                            if(e<0){
                                p1->y++;
                                e = e + dx;
                            }
                            p1->x++;
                        }

                    }
                    else {//donc de pi/4 à pi/2, octant numéro 2 (symétrique par rapport à x=y)

                        int e = dy;
                        dx = dx*2;
                        dy = e*2;

                        while(p1->y != p2->y + 1){
                            draw_position(p1,scene, color);
                            e = e - dx;
                            if(e<0){
                                p1->x++;
                                e = e + dy;
                            }
                            p1->y++;
                        }

                    }

                }
                else{//Donc Partie basse (toujours gauche)

                    if(dx>=-dy){//Octant numéro 8
                        
                        int e = dx;
                        dx = e*2;
                        dy = dy*2;

                        while(p1->x != p2->x + 1){
                            draw_position(p1,scene, color);
                            e = e + dy;
                            if(e<0){
                                p1->y--;
                                e = e + dx;
                            }
                            p1->x++;
                        }

                    }
                    else{//Octant numéro 7

                        int e = dy;
                        dx = dx*2;
                        dy = e*2;

                        while(p1->y != p2->y - 1){
                            draw_position(p1,scene, color);
                            e = e + dx;
                            if(e>0){
                                p1->x++;
                                e = e + dy;
                            }
                            p1->y--;
                        }

                    }
                }

            }
            else{//Horizontal à droite

                while(p1->x != p2->x + 1){
                    draw_position(p1,scene, color);
                    p1->x++;
                }
                
            }

        }
        else{//Partie Guauche

            if(dy!=0){//Non Horizontal

                if(dy>0){//Partie Haute-Guache

                    if(-dx >= dy){//Octant numéro 4

                        int e = dx;
                        dx = e*2;
                        dy = dy*2;

                        while(p1->x != p2->x - 1){
                            draw_position(p1,scene, color);
                            e = e + dy;
                            if(e>=0){
                                p1->y++;
                                e = e + dx;
                            }
                            p1->x--;
                        }

                    }
                    else{//Octane numéro 3

                        int e = dy;
                        dx = dx*2;
                        dy = e*2;

                        while(p1->y != p2->y + 1){
                            draw_position(p1,scene, color);
                            e = e + dx;
                            if(e<=0){
                                p1->x--;
                                e = e + dy;
                            }
                            p1->y++;
                        }

                    }

                }
                else{

                    if(dx<=dy){//Octant numéro 5

                        int e = dx;
                        dx = e*2;
                        dy = dy*2;

                        while(p1->x != p2->x - 1){
                            draw_position(p1,scene, color);
                            e = e - dy;
                            if(e>=0){
                                p1->y--;
                                e = e + dx;
                            }
                            p1->x--;
                        }

                    }
                    else{//Octant numéro 6

                        int e = dy;
                        dx = dx*2;
                        dy = e*2;

                        while(p1->y != p2->y - 1){
                            draw_position(p1,scene, color);
                            e = e - dx;
                            if(e>=0){
                                p1->x--;
                                e = e + dy;
                            }
                            p1->y--;
                        }

                    }

                }

            }
            else{//Horizontale Gauche

                while (p1->x != p2->x - 1){
                    draw_position(p1,scene,color);
                    p1->x--;
                }
                
            }

        }

    }
    else{//Vertical

        if(dy!=0){

            if(dy>0){

                while (p1->y != p2->y + 1){
                    draw_position(p1,scene,color);
                    p1->y++;
                }

            }
            else{

                while (p1->y != p2->y - 1){
                    draw_position(p1,scene,color);
                    p1->y--;
                }

            }

        }
        else{//p1 = p2

            draw_position(p1,scene,color);
        
        }

    }

    delete_position(p1);

}