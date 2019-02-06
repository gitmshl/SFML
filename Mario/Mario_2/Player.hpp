#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Anim.hpp"
#include "Consts.hpp"
#include "Bullet.hpp"
#include "Map.hpp"

using namespace sf;

class player
{
public:
    float x, y, dx, dy, w, h;
    Animation_Manager anim;
    bool onLadder, shoot, hit, onGround;
   int direction; /// направление движения (1 - направо, -1 - влево)

   enum {stay = 0, walk, duck, jump, climb, swim} STATE;
  /// std::map<std::string, bool> key;

    player (Animation_Manager &a, float X, float Y)
    {
        x = X;
        y = Y;
        dx = dy = 0;
        anim = a;
        STATE = stay;
        direction = 1;
        onGround = true;
    }

    void CollisionX();
    void CollisionY();

    void KeyCheck()
    {
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            if (STATE != duck) dx = -0.1;
            if (STATE == stay) STATE = walk;
            if (STATE != duck && STATE != climb) direction = -1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            if (STATE != duck) dx = 0.1;
            if (STATE == stay) STATE = walk;
            if (STATE != duck && STATE != climb) direction = 1;
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (STATE == stay || STATE == walk)
            {
                    dy = -0.27;
                    STATE = jump;
            }
            if (STATE == climb || STATE == swim) dy = -0.05;
            if (STATE == duck) STATE = stay;
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (STATE == stay || STATE == walk) STATE = duck; dx = 0;
            if (STATE == jump || STATE == swim) dy = 0.05;
        }

       if (Keyboard::isKeyPressed(Keyboard::Space))
        {
            shoot = true;
        }



        /// если клавиша отпущена

        if (!Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Right))
        {
            dx = 0;
            if (STATE == walk) STATE = stay;
        }
        if (!Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (STATE == swim || STATE == climb) dy = 0;
        }
        if (!Keyboard::isKeyPressed(Keyboard::Down))
        {
           if (STATE == duck)  STATE = stay;
        }
        if (!Keyboard::isKeyPressed(Keyboard::Space))
        {
           shoot = false;
        }
    }

    void update (float time)
    {
        KeyCheck();

        if (STATE == stay) anim.set("stay");
        if (STATE == walk) anim.set("walk");
        if (STATE == jump) { anim.set("jump"); onGround = false;}
        if (STATE == duck) anim.set("duck");
        if (STATE == climb)
        {
            anim.set("climb"); anim.pause();
            if (!dy) anim.play();
            if (!onLadder) STATE = stay; /// не понятно зачем и почему так!!!
        }

        if (shoot)
        {
            if (STATE == stay) anim.set("stay&&shoot");
            /// etc
        }

        if (direction == -1) anim.flip(true);
        else anim.flip(false);

        Animation *a;
        a = anim.animList.get_animation(anim.current_animation);
        int i = a->current_frame;
        w = a->frames[i].width;
        h = a->frames[i].height;

        x += dx * time;
        CollisionX();
        if (!onGround) dy += 0.0005 * time;

        y += dy * time;
        if (y > ground) {y = ground; dy = 0; onGround = true; STATE = stay;}
    CollisionY();


        anim.draw(window, x, y);
        anim.tick(time);

        dx = 0;
    }
};

void player::CollisionX()
{
    for (int i = y/10; i < (y+h)/10; i++)
    {
        for (int j = x/10; j < (x+w)/10; j++)
        {
            if (MAP[i][j] == 'P')
            {
                if (dx > 0) {x = j*10 - w; dx = 0;}
                if (dx < 0) {x = j*10 + 10; dx = 0;}
            }
        }
    }
}

void player::CollisionY()
{
    for (int i = y/10; i < (y+h)/10; i++)
    {
        for (int j = x/10; j < (x+w)/10; j++)
        {
            if (MAP[i][j] == 'P')
            {
                if (dy > 0) {y = i*10 - h; dy = 0; onGround = true;}
                if (dy < 0) {y = i*10 + 10; }
            }
        }
    }
}

#endif PLAYER_H // PLAYER_H

