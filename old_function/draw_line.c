#include "../entity/position.h"
#include "../scene/scene.h"
#include "../tools/draw.h"

void draw_line(position* p,position* p2,scene_object* scene,int color){

    position* p1 = copy_position(p);

    int dx = p2->x - p1->x;
    int dy = p2->y - p1->y;

    if (dx!=0){//Non-vertical

        if(dx>0){//Right side

            if(dy!=0){//Non-horizontal

                if(dy>0){//Top part, so from 0 to pi/2

                    if(dx >= dy){//So 0 to pi/4, octant number 1

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
                    else {//so from pi/4 to pi/2, octant number 2 (symmetrical with respect to x=y)

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
                else{//So bottom part (still left)

                    if(dx>=-dy){//Octant number 8
                        
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
                    else{//Octant number 7

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
            else{//Horizontal to the right

                while(p1->x != p2->x + 1){
                    draw_position(p1,scene, color);
                    p1->x++;
                }
                
            }

        }
        else{//Left side

            if(dy!=0){//Non-horizontal

                if(dy>0){//Top-Left part

                    if(-dx >= dy){//Octant number 4

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
                    else{//Octant number 3

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

                    if(dx<=dy){//Octant number 5

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
                    else{//Octant number 6

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
            else{//Horizontal Left

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