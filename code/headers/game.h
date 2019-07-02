//
//  game.h
//  Mario
//
//  Created by Илья Сельницкий on 14.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef game_h
#define game_h
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourcePath.hpp"
#include <iostream>
#include "animation.hpp"
#include "player.hpp"
#include "Entity.h"
#include "tinyxml.h"
#include "level.h"
#include "bonus.hpp"
#include "platform.h"
#include "enemy.hpp"
#include <sstream>
#include "healthbar.hpp"
#include "menu.hpp"

#define NumOfLevels 2

int score;

int abs(int a, int b)
{
    return a > b ? a - b : b - a;
}

template <typename T>
std::string toString(T val)
{
    std::ostringstream oss;
    oss<< val;
    return oss.str();
}
bool completed;
int start_game(sf::View &view,
               Level& level,
               sf::RenderWindow& window,
               sf::Texture& texture,
               sf::Texture& easyEnemyTexture,
               sf::Texture& platformTexture,
               AnimationManager& smallMarioAnim,
               AnimationManager& bigMarioAnim,
               AnimationManager& flyeMarioAnim,
               AnimationManager& dragonEnemy,
               AnimationManager& plantEnemy,
               AnimationManager& turtleStayEnemy,
               AnimationManager& turtleEnemy,
               AnimationManager& cacaEnemy,
               AnimationManager& easyEnemy,
               AnimationManager& bonusAnim,
               AnimationManager& platformAnim,
               int lives,
               sf::Sound &jump,
               sf::Sound &kikck,
               sf::Sound &down,
               sf::Sound &MarioSound
               )

{
    
    
    Player Mario(smallMarioAnim,bigMarioAnim,flyeMarioAnim,level, 10, 10);
    
    MarioSound.play();
    MarioSound.setLoop(true);
    Mario.numOfLives = lives;
    std::vector<Bonus> bonuses;
    std::vector<Entity*> entitys;
    
    sf::Font font;
    if ( !font.loadFromFile(resourcePath() + "sansation.ttf") )
    {
        return EXIT_FAILURE ;
    }
    sf::Text text("Score", font, 30);
    text.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Text scoreText("", font, 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    sf::Clock clock;
    int ofsetX,ofsetY, x, y;
    ofsetX = 0;
    ofsetY = Mario.y;
    Mario.score = score;
    x = y = 0;
    healthbar health;
    std::vector<Object> obj = level.GetAllObjects();
    for(int i = 0; i < obj.size(); i++)
    {
        if(obj[i].name == "platform")
        {
            entitys.push_back(new Platform(platformAnim, level, obj[i].rect.left, obj[i].rect.top));
        }
        if(obj[i].name == "turtle")
        {
            entitys.push_back(new EasyEnemy(turtleEnemy, level, obj[i].rect.left, obj[i].rect.top));
        }
        if(obj[i].name == "easy")
        {
            entitys.push_back(new EasyEnemy(easyEnemy, level, obj[i].rect.left, obj[i].rect.top));
        }
        if(obj[i].name == "fly")
        {
            entitys.push_back(new FlyEnemy(cacaEnemy, level, obj[i].rect.left, obj[i].rect.top));
        }
        if(obj[i].name == "MediumEnemy")
        {
            entitys.push_back(new MediumEnemy(turtleStayEnemy, level, obj[i].rect.left, obj[i].rect.top));
        }
    }
    while (window.isOpen())
    {
        
        window.clear(sf::Color(92,148,252));
        
        
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        
        time = time/500;
        
        if (time > 20) time = 20;
        
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
        }
        
        if(!window.isOpen())
            break;
        
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            Mario.key["L"]=true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            Mario.key["R"]=true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            Mario.key["Up"]=true;
            if(Mario.dy == 0)
                jump.play();
            //jump.setPlayingOffset(sf::seconds(3));
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            Mario.key["Down"]=true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            Mario.key["Space"]=true;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
        
        if(!window.isOpen())
            break;
        
        if(Mario.isNewBonus)
        {
            Bonus bonus(bonusAnim, Mario.bonusX, Mario.bonusY);
            if(bonus.kind != 6)
            {
                bonus.obj = Mario.obj;
                bonuses.push_back(bonus);
            }
            Mario.isNewBonus = false;
            Mario.bonusX = Mario.bonusY = 0;
        }
        level.SetObjects(Mario.obj);
        level.Draw(window);
        
        int numForErrase = -1;
        for(int i = 0; i < bonuses.size(); i++)
        {
            bonuses[i].update(time);
            bonuses[i].draw(window);
            if(Mario.getRect().intersects(bonuses[i].getRect()))
            {
                Mario.getBonus(bonuses[i].kind);
                numForErrase = i;
            }
            
        }
        if(numForErrase >= 0)
        {
            bonuses.erase(bonuses.begin() + numForErrase);
            numForErrase = -1;
        }
        
        for(int i = 0; i < entitys.size(); i++)
        {
            if(entitys[i] -> Name == "Platform")
            {
                if(Mario.getRect().intersects(entitys[i] -> getRect()) && Mario.life)
                {
                    
                    if(Mario.dy >= 0 && ((Mario.x > entitys[i] -> x - entitys[i] -> w / 2) || (Mario.x < entitys[i] -> x +  3/2*entitys[i] -> w)) &&
                       Mario.y + Mario.h < entitys[i] -> y+ entitys[i]->h)
                    {
                        Mario.x += entitys[i] -> dx * time;
                        Mario.y = entitys[i] -> y - Mario.h;
                        Mario.isInJump = false;
                        Mario.isFalling = false;
                        Mario.dy = 0;
                        Mario.STATE = Mario.STATE = Player::stay;
                        
                    }
                }
            }
            if((entitys[i] -> Name == "EasyEnemy" || entitys[i] -> Name == "fly" || entitys[i] -> Name == "MediumEnemy") && Mario.life)
            {
                if  (Mario.getRect().intersects( entitys[i]->getRect() ) && !entitys[i] -> del)
                {
                    if(Mario.dy>0 && Mario.life && entitys[i] -> Name == "MediumEnemy" && !entitys[i] -> life && entitys[i] -> timer > 1000)
                    {
                        entitys[i] -> del = true;
                    }
                    if (Mario.dy>0 && Mario.life && entitys[i] -> life)
                    {
                        entitys[i]->dx=0;
                        Mario.dy=-0.2;
                        entitys[i]->life=false;
                        entitys[i] -> dx =  0;
                        entitys[i] -> anim.set("dead");
                        kikck.play();
                        entitys[i] -> h = entitys[i] -> anim.getH();
                        entitys[i] -> w = entitys[i] -> anim.getW();
                        entitys[i] -> timer = 0;
                        Mario.score += 100;
                    }
                    else
                    {
                        if(entitys[i] -> life)
                        {
                            if(Mario.big)
                            {
                                Mario.big = false;
                                entitys[i]->life=false;
                                entitys[i] -> dx =  0;
                                entitys[i] -> anim.set("dead");
                                kikck.play();
                                entitys[i] -> h = entitys[i] -> anim.getH();
                                entitys[i] -> w = entitys[i] -> anim.getW();
                                Mario.score += 100;
                                
                            }
                            else
                            {
                                Mario.life = false;
                                down.play();
                                MarioSound.stop();
                            }
                        }
                    }
                }
            }
            
            entitys[i] -> obj = Mario.obj;
            entitys[i] -> update(time);
            if(!entitys[i] -> del)
                entitys[i] -> draw(window);
            
        }
        
        Mario.update(time);
        Mario.draw(window);
        
        
        if(Mario.x < 4500 * 8/9)
            view.setCenter( Mario.x + 250, 120);
        
        
        if(Mario.y > 225 && Mario.life)
        {
            Mario.life = false;
            down.play();
            MarioSound.stop();
            //break;
        }
        
        
        if(Mario.y > 240)
        {
            break;
        }
        health.update(Mario.numOfLives);
        window.setView(view);
        text.setPosition(view.getCenter().x + 200, 10);
        scoreText.setString(toString(Mario.score));
        scoreText.setPosition(view.getCenter().x + 200, 40);
        window.draw(text);
        window.draw(scoreText);
        health.draw(view.getCenter().x - 200,window);
        window.display();
        if(Mario.finish_start)
            MarioSound.stop();
        if(Mario.finish_finish)
            break;
        
        
    }
    
    for(int i = 0; i < entitys.size(); i++)
    {
        if(entitys[i] -> Name == "platform")
        {
            delete (Platform*)entitys[i];
        }
        if(entitys[i] -> Name == "EasyEnemy")
        {
            delete (EasyEnemy*)entitys[i];
        }
        if(entitys[i] -> Name == "fly")
        {
            delete (FlyEnemy*)entitys[i];
        }
        if(entitys[i] -> Name == "MediumEnemy")
        {
            delete (FlyEnemy*)entitys[i];
        }
        
        
    }
    
    while(Mario.finish.getStatus() != sf::Sound::Status::Stopped);
    
    if(!Mario.life){
        Mario.numOfLives--;
        
    }
    else
    {
        completed = true;
        score += Mario.score;
        float a;
        while(((a = clock.getElapsedTime().asSeconds())< 4));
        
    }
    
    while(down.getStatus() != sf::Sound::Status::Stopped);
    
    
    return Mario.numOfLives;
    
}
#endif /* game_h */
