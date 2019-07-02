//
//  Entity.h
//  Mario
//
//  Created by Илья Сельницкий on 05.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef Entity_h
#define Entity_h
#include "level.h"

class Entity
{
public:
    float x,y,dx,dy,w,h, timer;
    AnimationManager anim;
    bool life, dir, del;
    std::vector<Object> obj;
    std::string Name;
    
    Entity(AnimationManager &A, int X, int Y)
    {
        anim = A;
        x = X;
        y = Y;
        dir = 0;
        life = true;
        dx = dy = 0;
    }
    
    virtual void update(float time) = 0;
    
    void draw(sf::RenderWindow & window)
    {
        anim.draw(window ,x, y);
    }
    
    void option(std::string NAME, float SPEED=0, int HEALTH=10, std::string FIRST_ANIM="")
    {
        Name = NAME;
        if (FIRST_ANIM!="") anim.set(FIRST_ANIM);
        w = anim.getW();
        h = anim.getH();
        dx = SPEED;
    }
    
    sf::FloatRect getRect()
    {
        return sf::FloatRect(x,y,w,h);
    }

};
#endif /* Entity_h */
