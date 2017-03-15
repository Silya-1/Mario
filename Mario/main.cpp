#include "game.h"

int main()
{
    
    sf::SoundBuffer jumpbf;
    jumpbf.loadFromFile(resourcePath() + "Jump.ogg");//загружаем файл
    sf::Sound jump(jumpbf);
    jump.setVolume(20);
    
    sf::SoundBuffer kickbf;
    kickbf.loadFromFile(resourcePath() + "smb_kick.ogg");//загружаем файл
    sf::Sound kikck(kickbf);
    //kikck.play();
    
    sf::SoundBuffer downbf;
    downbf.loadFromFile(resourcePath() + "player_down.ogg");//загружаем файл
    sf::Sound down(downbf);
    //down.play();

    
    sf::SoundBuffer gameovbf;
    gameovbf.loadFromFile(resourcePath() + "game_over.ogg");//загружаем файл
    sf::Sound gameov(gameovbf);
    //down.play();

    
    sf::RenderWindow window2(sf::VideoMode(250 * 3, 223 * 3), "Start", sf::Style::Close);
    window2.setVerticalSyncEnabled(true);
    sf::Vector2i position(1000, 500);
    window2.setPosition(position);
    
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "Mario.png")) {
        return EXIT_FAILURE;
    }
    
    sf::SoundBuffer MaroBf;
    MaroBf.loadFromFile(resourcePath() + "Mario_Theme.ogg");//загружаем файл
    sf::Sound MarioSound(MaroBf);
    
    sf::SoundBuffer happyEndbf;
    happyEndbf.loadFromFile(resourcePath() + "smb_world_clear.ogg");//загружаем файл
    sf::Sound happyEnd(happyEndbf);
    
    
    window2.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    //window2.setFramerateLimit(70);

    //window2.clear();
    
    sf::View view( sf::FloatRect(0, 0, 750, 670));
    while (window2.isOpen())
    {
       
        window2.setView(view);
        window2.clear(sf::Color::Blue);
        sf::Event event;
        while (window2.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window2.close();
        }
       
        if(!window2.isOpen())
            break;
        sf::Texture texture;
        sf::Texture easyEnemyTexture;
        sf::Texture platformTexture;
    
        texture.loadFromFile(resourcePath() + "18133.png");
        easyEnemyTexture.loadFromFile(resourcePath() + "Mario_tileset.png");
        platformTexture.loadFromFile(resourcePath()+ "SMB-Tiles.gif");
    
        int numOfLevel = 1;
        Level* level = new Level();
        level -> LoadFromFile(resourcePath() + "try1.tmx");
    
        AnimationManager smallMarioAnim;
        AnimationManager bigMarioAnim;
        AnimationManager flyeMarioAnim;
    
        AnimationManager dragonEnemy;
        AnimationManager plantEnemy;
        AnimationManager turtleStayEnemy;
        AnimationManager turtleEnemy;
        AnimationManager cacaEnemy;
        AnimationManager easyEnemy;
        AnimationManager bonusAnim;
        AnimationManager platformAnim;
        
        smallMarioAnim.create("walk", texture,256, 8, 15, 15, 2, 0.005, 16);
        smallMarioAnim.create("stay", texture, 224, 8, 15, 15, 1, 0, 0);
        smallMarioAnim.create("jump", texture, 309, 8, 15, 15, 1, 0, 0);
        smallMarioAnim.create("remove", texture, 292, 8, 15, 15, 1, 0, 0);
    
        bigMarioAnim.create("stay", texture,158, 47, 18, 28, 1, 0, 0);
        bigMarioAnim.create("walk", texture, 194, 47, 18, 28, 2,  0.003, 18);
        bigMarioAnim.create("jump", texture, 270, 47, 18, 28, 1, 0, 0);
        bigMarioAnim.create("remove", texture, 252, 47, 18, 28, 1, 0, 0);
    
        flyeMarioAnim.create("stay", texture, 28, 251, 22, 30, 2, 0.001, 25);
        flyeMarioAnim.create("move", texture, 75, 238, 22, 40, 3, 0.005, 22);
        flyeMarioAnim.create("jump", texture, 165, 238, 22, 40, 1, 0, 0);
        flyeMarioAnim.create("remove", texture, 210, 238, 22, 40, 1, 0, 0);
    
        bonusAnim.create("stay", texture, 550, 995, 15, 15, 1, 0, 0);
    
        dragonEnemy.create("dragon_move", texture, 270, 600, 30, 40, 2, 0.003, 30);
        dragonEnemy.create("dragon_prepare_atack", texture, 430, 600, 30, 40, 1, 0, 0);
        dragonEnemy.create("dragon_atack", texture, 385, 600, 46, 30, 1, 0, 0);
    
    
        plantEnemy.create("stay", texture, 1, 832, 19, 35, 2, 0.0005, 19);
        turtleStayEnemy.create("move", texture, 109, 805, 17 , 26, 2, 0.003, 18);
        turtleStayEnemy.create("dead", texture, 74, 790, 19, 15, 1, 0, 1);
        turtleEnemy.create("move", texture, 1, 778, 20, 28, 2, 0.001, 18);
        turtleEnemy.create("dead", texture, 74, 790, 19, 15, 1, 0, 1);
    
        cacaEnemy.create("move", texture, 1, 716, 24, 23, 2, 0.001, 22);
        cacaEnemy.create("dead", texture, 81, 718, 20, 21, 1, 0, 0);
    
        easyEnemy.create("move", easyEnemyTexture, 0, 0, 18, 16, 2, 0.001, 18);
        easyEnemy.create("dead", easyEnemyTexture, 58, 0, 18, 16, 1, 0, 0);
    
        platformAnim.create("move", platformTexture, 68, 51, 15, 15, 1, 0, 0);
    
        sf::RenderWindow window(sf::VideoMode(600, 225), "MARIO", sf::Style::Fullscreen);
        window.setVisible(false);
        window2.setVisible(false);
        window2.setVisible(true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window2.close();
        if(start(window2, 1) == -1)
        {
            window2.close();
        }
        
        
        if(!window2.isOpen())
            break;
        window.setVisible(true);
        window2.setVisible(false);
        while (window.isOpen())
        {
            sf::View view1( sf::FloatRect(0, 0, 600, 225));
                window.setView(view1);
                window.setView(view);
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    window.close();
                }
            if(!window.isOpen())
                break;
            int live = 3;
            for(int i = 3; i >= 1; i--)
            {
        
                completed = false;
                live = start_game(view1,*level,window,texture,easyEnemyTexture,platformTexture,smallMarioAnim,               bigMarioAnim,flyeMarioAnim,
                   dragonEnemy,plantEnemy,turtleStayEnemy,turtleEnemy,cacaEnemy,easyEnemy, bonusAnim,   platformAnim, live, jump, kikck, down, MarioSound);
                if(completed)
                {
                    //printf("HOOORAY");
                    level -> ~Level();
                    level = new Level();
                    numOfLevel++;
                    if(numOfLevel > NumOfLevels)
                        break;
                    level -> LoadFromFile(resourcePath() + "try" + toString(numOfLevel) + ".tmx");
           
                }
                
                if(!window.isOpen())
                    break;
                i = (live) + 1;

            }
            if(!completed)
                gameov.play();
            else
                happyEnd.play();
            score = 0;
            window.close();
        }
        window2.setVisible(true);
        window2.clear(sf::Color::Blue);
    }
   
    return 0;
}
