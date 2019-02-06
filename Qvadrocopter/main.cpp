#include <SFML\Graphics.hpp>
#include <iostream>
#include "Qvadrocopter\Graphic.hpp"

using namespace sf;

void Draw(float X, float Y)
{
    RectangleShape r;
    r.setSize(Vector2f(30, 2));
    r.setPosition(X, Y);
    r.setFillColor(Color::Red);
    window.draw(r);
}

int main()
{
    float X = 200;
    float Y = 10;
    Qvadrakopter q;
    bool go = false;
    bool gameOver = false;

    Clock clock;

    while (window.isOpen() && !gameOver)
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

    if (Keyboard::isKeyPressed(Keyboard::K))
    {
        gameOver = true;
    }
    else if (Keyboard::isKeyPressed(Keyboard::G))
    {
        go = true;
    }

    if (go)
        q.Move(X, Y, time);

    Draw(X, Y);
    q.update(time);
    q.draw();

    window.display();

    }
    return 0;
}
