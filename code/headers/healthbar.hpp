//
//  healthbar.hpp
//  Mario
//
//  Created by Илья Сельницкий on 09.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef healthbar_h
#define healthbar_h

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ResourcePath.hpp"

class healthbar
{
public:
    sf::Texture red, grey;
    sf::Sprite h1,h2,h3;
    int max;
    
    healthbar()
    {
        red.loadFromFile(resourcePath() + "heathred.png");
        grey.loadFromFile(resourcePath() + "hearthgrey.png");
    }
    
    void update(int k)
    {
        switch (k) {
            case 1:
                h1.setTexture(red);
                h2.setTexture(grey);
                h3.setTexture(grey);
                break;
            case 2:
                h1.setTexture(red);
                h2.setTexture(red);
                h3.setTexture(grey);
                break;
            case 3:
                h1.setTexture(red);
                h2.setTexture(red);
                h3.setTexture(red);
                break;
            default:
                break;
        }
    }
    
   	void draw(float x, sf::RenderWindow &window)
    {
        sf::Vector2f center = window.getView().getCenter();
        sf::Vector2f size = window.getView().getSize();
        //printf("FF");
        h1.setScale(0.1f, 0.1f);
        h2.setScale(0.1f, 0.1f);
        h3.setScale(0.1f, 0.1f);

        x -= 40;
        h1.setPosition(x, 10);
        h2.setPosition((x) + h1.getTextureRect().width * 0.1f , 10);
        h3.setPosition((x) + 2* h1.getTextureRect().width * 0.1f , 10);
        
        window.draw(h1);
        window.draw(h2);
        window.draw(h3);
    }

    
};

#endif /* healthbar_h */
