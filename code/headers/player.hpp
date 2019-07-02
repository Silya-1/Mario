//
//  player.hpp
//  Mario
//
//  Created by Илья Сельницкий on 05.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef player_h
#define player_h

#include "Entity.h"
#include "bonus.hpp"


class Player:public Entity
{
public:
    
    bool hit, isInJump, isFalling, finish_start, finish_finish, big, flye;
    Level mainLevel;
    AnimationManager bigAnim, flyeAnim, normalAnim;
    int animType;//1 - normal   2 - big   3 - fly
    bool isNewBonus;
    int bonusX, bonusY;
    
    int numOfLives;
    int score;
    
    sf::SoundBuffer coinbf, upbf, bigbf, finishbf,breakBlockBf, bumpbf;
    sf::Sound coin, up, bigS, finish, breakBlock, bump;

    

    enum
    {
        stay,walk,jump,dye,remove
    } STATE;
    
    std::map<std::string, bool> key;
    
    Player(AnimationManager &a,AnimationManager &b,AnimationManager &c, Level &level, int x, int y):Entity(a,x,y)
    {
        option("Player",0,100,"stay");
        STATE=stay;
        obj = level.GetAllObjects();
        mainLevel = level;
        isInJump = true;
        isFalling = true;
        finish_start = finish_finish = false;
        big = false;
        flye = false;
        bigAnim = b;
        flyeAnim = c;
        normalAnim = a;
        isNewBonus = false;
        bonusX = bonusY = 0;
        score = 0;
        numOfLives = 1;
        animType = 1;
        
        coinbf.loadFromFile(resourcePath() + "smb_coin.ogg");//загружаем файл
        coin.setBuffer(coinbf);
        
        breakBlockBf.loadFromFile(resourcePath() + "smb_breakblock.ogg");
        breakBlock.setBuffer(breakBlockBf);
        
        upbf.loadFromFile(resourcePath() + "smb_powerup_appears.ogg");
        up.setBuffer(upbf);
        
        bigbf.loadFromFile(resourcePath() + "smb_powerup.ogg");
        bigS.setBuffer(bigbf);
        
        finishbf.loadFromFile(resourcePath() + "smb_stage_clear.ogg");
        finish.setBuffer(finishbf);
        
        bumpbf.loadFromFile(resourcePath() + "smb_bump.ogg");
        bump.setBuffer(bumpbf);
    }
    
    void KeyCheck()
    {
        if(!finish_start)
        {
            if(key["L"])
            {
                if(dx > 0)
                {
                    STATE = remove;
                    dx -= 0.05;
                    return;
                }
            
                dir = 1;

                if(STATE == stay || STATE == remove)
                    STATE = walk;
                dx = -0.2;
            }
        }
        if(key["R"])
        {
            
            if(!finish_start)
            {
                if(dx < 0)
                {
                    STATE = remove;
                    dx += 0.05;
                    return;
                }
                dir = 0;

                if(STATE == stay || STATE == remove)
                    STATE = walk;
                dx = 0.2;
            }
        }
        if(key["Up"])
        {
            if(!finish_start)
            {
                if((STATE == stay || STATE == walk) && (!isInJump))
                {
                    dy = -0.5;
                    STATE = jump;
                    isInJump = true;
                }
            }
        }
        if(!(key["R"] || key["L"]))
        {
            if(!finish_start)
            {
                dx = 0;
                if(STATE == walk)
                STATE = stay;
            }
            
        }
    }
    
    void update(float time)
    {
        if(big && (animType != 2))
        {
            anim = bigAnim;
            animType = 2;
            h = bigAnim.getH();
            w = bigAnim.getW();
            y -= bigAnim.getH() - normalAnim.getH();
        }
        if(flye && (animType != 3))
        {
            anim = bigAnim;
            animType = 3;
            h = flyeAnim.getH();
            w = flyeAnim.getW();
            y -= flyeAnim.getH() - normalAnim.getH();
        }
        if(!big && (animType != 1))
        {
            animType = 1;
            h = normalAnim.getH();
            w = normalAnim.getW();
            y += anim.getH() - normalAnim.getH();
            anim = normalAnim;
            
        }

        KeyCheck();
        
        if(STATE == stay)
            anim.set("stay");
        if(STATE == walk)
            anim.set("walk");
        if(STATE == jump)
            anim.set("jump");
        if(STATE == remove)
            anim.set("remove");
        
        if(life)
        {
            anim.flip(dir);
            x += dx * time;
            Collision(0);
        }
        if(life)
            dy += 0.002 * time;
        else
            dy = 0.003 * time;
        y += dy * time;
        if(dy > 0)
            isFalling = true;
        if(life)
            Collision(1);
        
        anim.tick(time);
        key["R"]=key["L"]=key["Up"]=key["Down"]=key["Space"]=false;
        
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

                if(obj[i].name=="finish_start")
                {
                    finish_start = true;
                    dx = 0.1;
                    finish.play();
                   
                    STATE = walk;
                }
                
                if(obj[i].name=="finish_end")
                {
                    finish_finish = true;
                    anim.set("stay");
                    dx = 0;
                }
                
                if(obj[i].name == "coin")
                {
                    obj[i].name = "destroy";
                    coin.play();
                    score += 100;
                }
                if (obj[i].name=="solid" || obj[i].name=="bonus" || obj[i].name == "bord" || obj[i].name=="empty")
                {
                    if (dy>0 && num==1)
                    {
                       
                        y = obj[i].rect.top -  h;
                        move_dy = 0;
                        dy=0;
                        if(!finish_start)
                        {
                            STATE=stay;
                        }
                        isInJump = false;
                        isFalling = false;
                    }
                 
                    if (dy< 0 && num==1)
                    {
                        if(obj[i].name == "bonus")
                        {
                            obj[i].name = "empty";
                            isNewBonus = true;
                            bonusY = obj[i].rect.top -  1;
                            bonusX = obj[i].rect.left;
                            up.play();
                        }
                        if( (obj[i].name == "solid" || obj[i].name == "empty") && big)
                        {
                            obj[i].name = "destroy";
                            breakBlock.play();
                        }
                        y = obj[i].rect.top + obj[i].rect.height ;
                        move_dy = 0;
                        bump.play();
                      
                    }
                    if (dx>0 && num==0)
                    {
                        x =  obj[i].rect.left -  w;
                    }
                    if (dx<0 && num==0)
                    {
                        x =  obj[i].rect.left + obj[i].rect.width;
                    }
                    
                }
            }
            
          
        }
          dy = move_dy;
    }
    
    
    void getBonus(int kind)
    {
        switch (kind)
        {
            case 1:
                numOfLives ++;
                if(numOfLives > 3)
                    numOfLives = 3;
                break;
            case 2:
                big = true;
                bigS.play();
                break;
            case 3:
                coin.play();
                score += 200;
                break;
            case 4:
                coin.play();
                score += 500;
                break;
            case 5:
                coin.play();
                score += 100;
                break;
            default:
                coin.play();
                break;
        }
    }
};
#endif /* player_h */
