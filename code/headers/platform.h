//
//  platform.h
//  Mario
//
//  Created by Илья Сельницкий on 08.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef platform_h
#define platform_h

#include "Entity.h"
#include "bonus.hpp"

class Platform : public Entity
{
public:
    Platform(AnimationManager &a, Level &lev,int x,int y):Entity(a,x,y)
    {
        option("Platform", 0.05, 0, "move");
        del = false;
        obj = lev.GetAllObjects();
    }
    
    void update(float time)
    {
        x += dx * time;
        Collision();
        anim.tick(time);
    }
    
    void Collision()
    {
        
        sf::FloatRect rect = getRect();
        for (int i = 0; i < obj.size(); i++)
        {
            
            sf::FloatRect a = getObjRect(obj[i]);
            if(a.intersects(rect) && obj[i].name != "platform")
            {
                if (dx>0)
                {
                    x =  obj[i].rect.left -  w;
                    dx = -dx;
                    break;
                }
                if (dx<0)
                {
                    x =  obj[i].rect.left + obj[i].rect.width;
                    dx = -dx;
                    break;
                }
            }
        }
    }
};


#endif /* platform_h */
