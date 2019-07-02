//
//  level.h
//  Mario
//
//  Created by Илья Сельницкий on 06.03.17.
//  Copyright © 2017 Илья Сельницкий. All rights reserved.
//

#ifndef level_h
#define level_h

#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-system.lib")

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "tinyxml.h"
#include <iostream>


struct Object
{
    int GetPropertyInt(std::string name);
    float GetPropertyFloat(std::string name);
    std::string GetPropertyString(std::string name);
    
    std::string name;
    std::string type;
    sf::Rect<int> rect;
    std::map<std::string, std::string> properties;
    bool alive;
    bool bonus;
    sf::Sprite sprite;
};


sf::FloatRect getObjRect(Object a)
{
    return (sf::FloatRect)a.rect;
}

struct Layer
{
    int opacity;
    std::vector<sf::Sprite> tiles;
};

class Level
{
public:
    bool LoadFromFile(std::string filename);
    Object GetObject(std::string name);
    std::vector<Object> GetAllObjects();
    std::vector<Object> GetObjects(std::string name);
    void Draw(sf::RenderWindow &window);
    sf::Vector2i GetTileSize();
    int GetWidth();
    int GetHeight();
    void  SetObjects(std::vector<Object> obj);

    
private:
    int width, height, tileWidth, tileHeight, space;
    int firstTileID;
    sf::Rect<float> drawingBounds;
    sf::Texture tilesetImage;
    std::vector<Object> objects;
    std::vector<Layer> layers;
};

void  Level::SetObjects(std::vector<Object> obj)
{
    objects = obj;
}


int Object::GetPropertyInt(std::string name)
{
    return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name)
{
    return strtod(properties[name].c_str(), NULL);
}

std::string Object::GetPropertyString(std::string name)
{
    return properties[name];
}

bool Level::LoadFromFile(std::string filename)
{
    space = 0;
    TiXmlDocument levelFile(filename.c_str());
    
    // Загружаем XML-карту
    if(!levelFile.LoadFile())
    {
        std::cout << "Loading level \"" << filename << "\" failed." << std::endl;
        return false;
    }
    
    // Работаем с контейнером map
    TiXmlElement *map;
    map = levelFile.FirstChildElement("map");
    
    // Пример карты: <map version="1.0" orientation="orthogonal"
    // width="10" height="10" tilewidth="34" tileheight="34">
    width = atoi(map->Attribute("width"));
    height = atoi(map->Attribute("height"));
    tileWidth = atoi(map->Attribute("tilewidth"));
    tileHeight = atoi(map->Attribute("tileheight"));
    
    // Берем описание тайлсета и идентификатор первого тайла
    TiXmlElement *tilesetElement;
    tilesetElement = map->FirstChildElement("tileset");
    firstTileID = atoi(tilesetElement->Attribute("firstgid"));
    space = atoi(tilesetElement->Attribute("spacing"));

    
    // source - путь до картинки в контейнере image
    TiXmlElement *image;
    image = tilesetElement->FirstChildElement("image");
    
   // image = tilesetElement->FirstChildElement("image");
    //std::string imagepath = image->Attribute("source");

    
    std::string path = resourcePath();
    std::string imagepath =  resourcePath() + "SMB-Tiles.gif";
    
    // Пытаемся загрузить тайлсет
    sf::Image img;
    
    if(!img.loadFromFile(imagepath))
    {
        std::cout << "Failed to load tile sheet." << std::endl;
        return false;
    }
    
    // Очищаем карту от света (109, 159, 185)
    // Вообще-то в тайлсете может быть фон любого цвета, но я не нашел решения, как 16-ричную строку
    // вроде "6d9fb9" преобразовать в цвет
    //img.createMaskFromColor(sf::Color(109, 159, 185));
    // Грузим текстуру из изображения
    tilesetImage.loadFromImage(img);
    // Расплывчатость запрещена
    tilesetImage.setSmooth(false);
    
    // Получаем количество столбцов и строк тайлсета
    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;
    
    // Вектор из прямоугольников изображений (TextureRect)
    std::vector<sf::Rect<int>> subRects;
    
    for(int y = 0; y < rows - 1; y++)
        for(int x = 0; x < columns - 1; x++)
        {
            sf::Rect<int> rect;
            rect.top = y * tileHeight + y * space;
            rect.height = tileHeight;
            rect.left = x * tileWidth + x * space;
            rect.width = tileWidth;
            subRects.push_back(rect);
        }
    
    // Работа со слоями
    TiXmlElement *layerElement;
    layerElement = map->FirstChildElement("layer");
    while(layerElement)
    {
        Layer layer;
        
        // Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
        if (layerElement->Attribute("opacity") != NULL)
        {
            float opacity = strtod(layerElement->Attribute("opacity"), NULL);
            layer.opacity = 255 * opacity;
        }
        else
        {
            layer.opacity = 255;
        }
        
        // Контейнер <data>
        TiXmlElement *layerDataElement;
        layerDataElement = layerElement->FirstChildElement("data");
        
        if(layerDataElement == NULL)
        {
            std::cout << "Bad map. No layer information found." << std::endl;
        }
        
        // Контейнер <tile> - описание тайлов каждого слоя
        TiXmlElement *tileElement;
        tileElement = layerDataElement->FirstChildElement("tile");
        
        if(tileElement == NULL)
        {
            std::cout << "Bad map. No tile information found." << std::endl;
            return false;
        }
        
        int x = 0;
        int y = 0;
        
        while(tileElement)
        {
            int tileGID = atoi(tileElement->Attribute("gid"));
            int subRectToUse = tileGID - firstTileID;
        

            
            // Устанавливаем TextureRect каждого тайла
            if (subRectToUse >= 0)
            {
                sf::Sprite sprite;
                sprite.setTexture(tilesetImage);
                sprite.setTextureRect(subRects[subRectToUse]);
                sprite.setPosition(x * tileWidth, y * tileHeight);
                sprite.setColor(sf::Color(255, 255, 255, layer.opacity));
                
                layer.tiles.push_back(sprite);
            }
            
            tileElement = tileElement->NextSiblingElement("tile");
            
            x++;
            if (x >= width)
            {
                x = 0;
                y++;
                if(y >= height)
                    y = 0;
            }
        }
        
        layers.push_back(layer);
        
        layerElement = layerElement->NextSiblingElement("layer");
    }
    
    // Работа с объектами
    TiXmlElement *objectGroupElement;
    
    // Если есть слои объектов
    if (map->FirstChildElement("objectgroup") != NULL)
    {
        objectGroupElement = map->FirstChildElement("objectgroup");
        while (objectGroupElement)
        {
            // Контейнер <object>
            TiXmlElement *objectElement;
            objectElement = objectGroupElement->FirstChildElement("object");
            
            while(objectElement)
            {
                // Получаем все данные - тип, имя, позиция, etc
                std::string objectType;
                if (objectElement->Attribute("type") != NULL)
                {
                    objectType = objectElement->Attribute("type");
                }
                std::string objectName;
                if (objectElement->Attribute("name") != NULL)
                {
                    objectName = objectElement->Attribute("name");
                }
                int x = atoi(objectElement->Attribute("x"));
                int y = atoi(objectElement->Attribute("y"));
                
                int width, height;
                
                sf::Sprite sprite;
                sprite.setTexture(tilesetImage);
                sprite.setTextureRect(sf::Rect<int>(0,0,0,0));
                sprite.setPosition(x, y);
                
                if (objectElement->Attribute("width") != NULL)
                {
                    width = atoi(objectElement->Attribute("width"));
                    height = atoi(objectElement->Attribute("height"));
                }
                else
                {
                    width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
                    height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
                    sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
                }
                
                // Экземпляр объекта
                Object object;
                object.name = objectName;
                object.type = objectType;
                object.sprite = sprite;
                object.alive = 1;
                if(std::strcmp(object.name.data(), "bonus") == 0)
                    object.bonus = 1;
                else
                    object.bonus = 0;
                sf::Rect <int> objectRect;
                objectRect.top = y;
                objectRect.left = x;
                objectRect.height = height;
                objectRect.width = width;
                object.rect = objectRect;
                
                // "Переменные" объекта
                TiXmlElement *properties;
                properties = objectElement->FirstChildElement("properties");
                if (properties != NULL)
                {
                    TiXmlElement *prop;
                    prop = properties->FirstChildElement("property");
                    if (prop != NULL)
                    {
                        while(prop)
                        {
                            std::string propertyName = prop->Attribute("name");
                            std::string propertyValue = prop->Attribute("value");
                            
                            object.properties[propertyName] = propertyValue;
                            
                            prop = prop->NextSiblingElement("property");
                        }
                    }
                }
                
                // Пихаем объект в вектор
                objects.push_back(object);
                
                objectElement = objectElement->NextSiblingElement("object");
            }
            objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
    else
    {
        std::cout << "No object layers found..." << std::endl;
    }
    
    return true;
}


Object Level::GetObject(std::string name)
{
    // Только первый объект с заданным именем
    for (int i = 0; i < objects.size(); i++)
        if (objects[i].name == name)
            return objects[i];
}

std::vector<Object> Level::GetObjects(std::string name)
{
    // Все объекты с заданным именем
    std::vector<Object> vec;
    for(int i = 0; i < objects.size(); i++)
        if(objects[i].name == name)
            vec.push_back(objects[i]);
    
    return vec;
}

std::vector<Object> Level::GetAllObjects()
{
    return objects;
}

sf::Vector2i Level::GetTileSize()
{
    return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow &window)
{
    // Рисуем все тайлы (объекты НЕ рисуем!)
    for(int layer = 0; layer < layers.size(); layer++)
        for(int tile = 0; tile < layers[layer].tiles.size(); tile++)
            window.draw(layers[layer].tiles[tile]);
    
    sf::Texture tileSet;
    tileSet.loadFromFile(resourcePath() + "Mario_tileset.png");
    sf::Sprite tile(tileSet);
    tile.setTextureRect(sf::IntRect(144-16,113,15,15));
    
    sf::Texture tileForCoin;
    tileForCoin.loadFromFile(resourcePath() + "18133.png");
    sf::Sprite coin(tileForCoin);
    coin.setTextureRect(sf::IntRect(525, 535, 15, 15));
    

    for(int i = 0; i < objects.size(); i++)
    {
        if(objects[i].name == "bonus")
        {
            tile.setPosition(objects[i].rect.left, objects[i].rect.top );
            window.draw(tile);
        }
        if(objects[i].name == "coin")
        {
            coin.setPosition(objects[i].rect.left, objects[i].rect.top);
            window.draw(coin);

        }
        if(objects[i].name == "destroy")
        {
            sf::RectangleShape rec(sf::Vector2f(15, 15));
            rec.setPosition(objects[i].rect.left, objects[i].rect.top );
            rec.setFillColor(sf::Color(92,148,252));
            
            window.draw(rec);
        }
    }
}

#endif /* level_h */
