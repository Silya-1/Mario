//
//  enemy.hpp
//  Mario
//
//  Created by Илья Сельницкий on 08.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef enemy_h
#define enemy_h

#include "Entity.h"

class EasyEnemy : public Entity
{
public:
    EasyEnemy(AnimationManager &a,Level &lev,int x,int y):Entity(a,x,y)
    {
        option("EasyEnemy", 0, 15, "move");
        del = false;
        obj = lev.GetAllObjects();
    }
    
    void update(float time)
    {
        if(dx == 0 && dy == 0 && life)
        {
            dx = 0.03;
        }
        x += dx * time;
        
        anim.flip(dir);
        Collision(0);
        dy += 0.002 * time;
        y += dy * time;
        Collision(1);
        timer += time;
        if (timer > 3000)
        {
            timer = 0;
            if(!life)
            {
                del = true;
            }
        }
        if(!life)
        {
            anim.set("dead");
        }
        anim.tick(time);
        
    }
    
    
    void Collision(int num)
    {
        sf::FloatRect rect = getRect();
        float move_dy = dy;
        for (int i = 0; i < obj.size(); i++)
        {
            
            sf::FloatRect a = getObjRect(obj[i]);
            
            if (a.intersects(rect))
            {
                
                if (obj[i].name=="solid" || obj[i].name=="bonus" || obj[i].name == "border" || obj[i].name=="empty" || obj[i].name == "bord")
                {
                    if (dy>0 && num==1)
                    {
                        y = obj[i].rect.top -  h;
                        move_dy = 0;
                        dy=0;
                    }
                    
                    if (dx>0 && num==0)
                    {
                        x =  obj[i].rect.left -  w;
                        dx = -dx;
                        dir = true;
                        break;
                    }
                    if (dx<0 && num==0)
                    {
                        x =  obj[i].rect.left + obj[i].rect.width;
                        dx = -dx;
                        dir = false;
                        break;
                    }
                    
                }
            }
            
            
        }
        dy = move_dy;
    }
    
};

class FlyEnemy : public Entity
{
public:
    FlyEnemy(AnimationManager &a,Level &lev,int x,int y):Entity(a,x,y)
    {
        option("fly", 0, 15, "move");
        del = false;
        obj = lev.GetAllObjects();
    }
    
    void update(float time)
    {
        if(dx == 0 && dy == 0 && life)
        {
            dx = 0.04;
        }
        x += dx * time;
        
        anim.flip(dir);
        Collision(0);
        if(!life)
        {
            dy += 0.002 * time;
            y += dy * time;
            Collision(1);
        }
        
        timer += time;
        if (timer > 3000)
        {
            timer = 0;
            if(!life)
            {
                del = true;
            }
        }
        if(!life)
        {
            anim.set("dead");
        }
        anim.tick(time);
        
    }

    void Collision(int num)
    {
        sf::FloatRect rect = getRect();
        float move_dy = dy;
        for (int i = 0; i < obj.size(); i++)
        {
            
            sf::FloatRect a = getObjRect(obj[i]);
            
            if (a.intersects(rect))
            {
                
                if (obj[i].name=="solid" || obj[i].name=="bonus" || obj[i].name == "border" || obj[i].name=="empty" || obj[i].name == "bord")
                {
                    if (dy>0 && num==1)
                    {
                        y = obj[i].rect.top -  h;
                        move_dy = 0;
                        dy=0;
                    }
                    
                    if (dx>0 && num==0)
                    {
                        x =  obj[i].rect.left -  w;
                        dx = -dx;
                        dir = true;
                        break;
                    }
                    if (dx<0 && num==0)
                    {
                        x =  obj[i].rect.left + obj[i].rect.width;
                        dx = -dx;
                        dir = false;
                        break;
                    }
                    
                }
            }
            
            
        }
        dy = move_dy;
    }

};


class MediumEnemy : public Entity
{
public:
    MediumEnemy(AnimationManager &a,Level &lev,int x,int y):Entity(a,x,y)
    {
        option("MediumEnemy", 0, 15, "move");
        del = false;
        obj = lev.GetAllObjects();
    }
    
    void update(float time)
    {
        if (timer > 3000)
        {
            timer = 0;
            if(!life)
            {
                float height;
                height = anim.getH();
                anim.set("move");
                y  -= 3 * (anim.getH() - h);
                h = anim.getH();
                w = anim.getW();
                dir = false;
                life = true;
            }
        }
        if(!life)
        {
            h = anim.getH();
            anim.set("dead");
            y  += anim.getH() - h;
        }

        
        if(dx == 0 && dy == 0 && life)
        {
            dx = 0.02;
        }
        x += dx * time;
        
        anim.flip(dir);
        Collision(0);
        dy += 0.002 * time;
        y += dy * time;
        Collision(1);
        timer += time;
        anim.tick(time);
        
    }
    
    void Collision(int num)
    {
        sf::FloatRect rect = getRect();
        float move_dy = dy;
        for (int i = 0; i < obj.size(); i++)
        {
            
            sf::FloatRect a = getObjRect(obj[i]);
            
            if (a.intersects(rect))
            {
                
                if (obj[i].name=="solid" || obj[i].name=="bonus" || obj[i].name == "border" || obj[i].name=="empty" || obj[i].name == "bord")
                {
                    if (dy>0 && num==1)
                    {
                        y = obj[i].rect.top -  h;
                        move_dy = 0;
                        dy=0;
                    }
                    
                    if (dx>0 && num==0)
                    {
                        x =  obj[i].rect.left -  w;
                        dx = -dx;
                        dir = true;
                        break;
                    }
                    if (dx<0 && num==0)
                    {
                        x =  obj[i].rect.left + obj[i].rect.width;
                        dx = -dx;
                        dir = false;
                        break;
                    }
                    
                }
            }
            
            
        }
        dy = move_dy;
    }

};


#endif /* enemy_h */
