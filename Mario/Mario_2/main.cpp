#include <SFML/Graphics.hpp>
#include "Mario/Player.hpp"

int main()
{
    Texture t;
    t.loadFromFile("fang.png");


    Animation_Manager anim;

    anim.create("stay", t, 0, 190, 43, 51, 3, 0.002);
    anim.create("walk", t, 0, 244, 40, 50, 6, 0.005);
    anim.create("jump", t, 0, 1015, 37, 33, 4, 0.005);
    anim.create("duck", t, 0, 1426, 46, 44, 1, 0);
    anim.create("stay&&shoot", t, 0, 574, 45, 50, 5, 0.005);

    player p(anim, 100, ground);

    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 360;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

    window.clear();

        Map_Draw();

        p.KeyCheck();
        p.update(time);

        window.display();

    }

    return 0;
}
