#ifndef MAP_H
#define MAP_H

#include<string>
#include "Consts.hpp"
#include <SFML/Graphics.hpp>

using namespace sf;

std::string MAP[Height_Map] =
{
    "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                                                                                                                      P",
    "P                  PPP                                                                                                 P",
    "P                  PPP                                                                                                 P",
    "P                  PPP                                                                                                 P",
    "P                  PPP                                                                                                 P",
    "P                  PPP                                                                                                 P",
    "P                  PPP                                                                                                 P",
    "P                  PPP                                                                                                 P",
    "PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",

};


void Map_Draw()
{
    Texture t;
    t.loadFromFile("Mario_tileset.png");

    Sprite sp;
    sp.setTexture(t);

    sp.setTextureRect(IntRect(224, 192, 10, 10));

    for (int i = 0; i < Height_Map; i++)
        for (int j = 0; j < Width_Map; j++)
        {
            if (MAP[i][j] == 'P')
            {
                sp.setPosition(j*10, i*10);
                window.draw(sp);
            }
        }
}

#endif  MAP_H
