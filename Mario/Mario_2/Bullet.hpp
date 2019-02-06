#ifndef BULLET_H
#define BULLET_H

#include "Anim.hpp"
#include "Consts.hpp"
#include "Entiti.hpp"

class bullet : public entiti
{
public:

    bullet(Animation_Manager &a, float X, float Y, int dir) : entiti(a, X, Y, dir)
    {
        dx = 0.3 * dir;
        dy = 0;
    }

    void Collision();

    void update(float time)
    {
        x += dx * time;

        bool hit = CollisionX();

        if (hit) life = false;
        if (x > Max_X_Coordinate || x < -Max_X_Coordinate) life = false;
        anim.tick(time);
        draw();
    }
};



#endif BULLET_H
