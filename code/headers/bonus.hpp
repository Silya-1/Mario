//
//  bonus.hpp
//  Mario
//
//  Created by Илья Сельницкий on 06.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef bonus_h
#define bonus_h


#include <iostream>
#include "Entity.h"

class Bonus :public Entity
{
public:
    int kind;//1 - life (7) , 2 - big (2),  3 - -100(3 and 2), 4 - death(11), 5 - +100(3 and not 2), 6 nouth
    int startX, startY;
    bool move;
    
    Bonus(AnimationManager &a, int X, int Y):Entity(a,x,y)
    {
        int num = rand();
        if(num % 2 == 0)
            kind = 2;
        else if(num % 7 == 0)
            kind = 1;
        else if(num % 6 == 0)
            kind = 3;
        else if(num % 3 == 0 && num % 2 != 0 )
            kind = 5;
        else if(num % 11 == 0)
            kind = 4;
        else kind = 5;
        
        startX = X;
        startY = Y;
        x = X;
        y = Y;
        w = h = 15;
        anim = a;
        dy = -0.4;
        move = true;
        anim.set("stay");
    }
    
    void update(float time)
    {
        if(y <  (startY - 15))
        {
            move = false;
            y = startY - 15;
            dy = 0;
        }
        if(!move)
        {
            Collision();
            dy += 0.005 * time;
            y += dy * time;
        }else
        {
            y += dy;
        }
        
        anim.tick(time);
    }
    
    void Collision()
    {
        for (int i = 0; i < obj.size(); i++)
        {
            sf::FloatRect rect = getRect();
            sf::FloatRect a = getObjRect(obj[i]);
            
            if (a.intersects(rect))
            {
                if (obj[i].name=="solid" || obj[i].name=="bonus" || obj[i].name == "border" || obj[i].name=="empty")
                {
                    if (dy>0)
                    {
                        y = obj[i].rect.top -  h;
                        dy=0;
                    }
                }
 
            }
        }
    }
    
};


#endif /* bonus_h */
