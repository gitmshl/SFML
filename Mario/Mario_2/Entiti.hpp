#ifndef ENTITI_H
#define ENTITI_H

#include "Consts.hpp"
#include "Map.hpp"

class entiti
{
public:
    float x, y, dx, dy, w, h;
    bool life;
    int dir;
    Animation_Manager anim;

    entiti(Animation_Manager &a, int X, int Y, int Dir)
    {
        anim = a;
        x = X;
        y = Y;
        dir = Dir;
        life = true;
    }

    entiti(Animation_Manager &a, int X, int Y)
    {
        anim = a;
        x = X;
        y = Y;
        dir = 1;
        life = true;
    }

    bool CollisionX();
    bool CollisionY();

    void draw ()
    {
        anim.draw(window, x, y);
    }

    virtual void update(float time) = 0;


};

bool entiti::CollisionX()
{
    bool flag = false;

    Animation *a;
        a = anim.animList.get_animation(anim.current_animation);
        int i = a->current_frame;
        w = a->frames[i].width;
        h = a->frames[i].height;

    for (int i = y/10; i < (y+h)/10; i++)
    {
        for (int j = x/10; j < (x+w)/10; j++)
        {
            if (MAP[i][j] == 'P')
            {
                flag = true;
                if (dx > 0) {x = j*10 - w; dx = 0;}
                if (dx < 0) {x = j*10 + 10; dx = 0;}
            }
        }
    }

    return flag;
}


bool entiti::CollisionY()
{
    bool flag = false;

    Animation *a;
        a = anim.animList.get_animation(anim.current_animation);
        int i = a->current_frame;
        w = a->frames[i].width;
        h = a->frames[i].height;

    for (int i = y/10; i < (y+h)/10; i++)
    {
        for (int j = x/10; j < (x+w)/10; j++)
        {
            if (MAP[i][j] == 'P')
            {
                flag = true;
                if (dy > 0) {y = i*10 - h; dy = 0; }
                if (dy < 0) {y = i*10 + 10; }
            }
        }
    }

     return flag;

}

#endif  ENTITI_H
