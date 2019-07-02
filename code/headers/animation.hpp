#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourcePath.hpp"
#include <iostream>

class Animation
{
public:
    std::vector<sf::IntRect> frames, frames_flip;
    float currentFrame, speed;
    bool flip, isPlaying;
    sf::Sprite sprite;
    
    void Animation_create (sf::Texture &t, int x, int y, int w, int h, int count, float Speed, int step)
    {
        speed = Speed;
        sprite.setTexture(t);
        
        currentFrame = 0;
        isPlaying = true;
        flip = false;
        
        for(int i = 0; i < count; i++)
        {
            frames.push_back(sf::IntRect(x + i * step, y, w, h));
            frames_flip.push_back(sf::IntRect(x + i * step + w, y, -w, h));
        }
    }
    
    void tick(float time)
    {
        if(!isPlaying) return;
        
        currentFrame += speed * time;
        
        if(currentFrame > frames.size())
            currentFrame -= frames.size();
        
        int i = currentFrame;
        
        sprite.setTextureRect(frames[i]);
        if(flip)
            sprite.setTextureRect(frames_flip[i]);
    }
};

class AnimationManager
{
public:
    sf::String currentAnim;
    std::map<sf::String, Animation> animList;
    
    AnimationManager()
    {}
    
    void create(std::string name, sf::Texture &texture, int x, int y, int w, int h, int count,float speed, int step)
    {
        Animation anim;
        anim.Animation_create(texture, x, y, w, h, count, speed, step);
        animList[name] = anim;
        currentAnim = name;
    }
    
    void draw(sf::RenderWindow &window, int x = 0, int y = 0)
    {
        animList[currentAnim].sprite.setPosition(x,y);
        window.draw( animList[currentAnim].sprite);
    }
    
    void set(sf::String name)
    {
        currentAnim = name;
    }
    
    void flip(bool b)
    {
        animList[currentAnim].flip = b;
    }
    
    void tick(float time)
    {
        animList[currentAnim].tick(time);
    }
    
    void pause()
    {
        animList[currentAnim].isPlaying = false;
    }
    
    void play()
    {
        animList[currentAnim].isPlaying = true;
    }
    
    float getH()  {return animList[currentAnim].frames[0].height;}
    
    float getW() {return animList[currentAnim].frames[0].width;}

};
#endif
