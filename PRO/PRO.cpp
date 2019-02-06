#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <windows.h>

using namespace sf;

const int Width = 900;
const int Height = 650;
const float ground = Height-30;
const int rocket_size = 2;
const float a_max = 0.0003;  /// 0.0003
const float a_min = 0.0001;
const float const_bang_wave_speed = 0.04;
const float dt = 1;
const float min_distance = 3;
const float speed = 0.04;   /// 0.044 (or 0.01)

bool gameOver = false;  /// if true - конец игры

RenderWindow window (VideoMode(Width, Height), "PRO");
CircleShape rocket_body(rocket_size);
CircleShape rocket_PRO_body(rocket_size);

bool rocket_launch = false; /// разрешение на выпуск ракеты пво в воздух
bool bang = false; /// разрешение на взрыв(если тру, то взрыв произошел)


int sgn(float a)
{
    if (a > 0) return 1;
    if (a < 0) return -1;
    return 0;
}

class bang_wave /// ударная волна
{
public:
    float x, y; /// координаты
    float bang_wave_speed;
    float radius_wave;
    bang_wave(float X, float Y, float b_w_s = const_bang_wave_speed)
    {
        x = X;
        y = Y;
        bang_wave_speed = b_w_s;
        radius_wave = 0;
    }

    void update(float time)
    {
        radius_wave += bang_wave_speed * time;
        CircleShape c((radius_wave));
        c.setPosition(x, y);
        window.draw(c);
    }

};

bang_wave b(0, 0);


class rocket
{
public:

    float x, y; /// координаты
    float v_x, v_y; /// скорость
    float a_x, a_y; /// ускорение

    rocket(float X = Width-rocket_size, float Y = Height-rocket_size,
           float V_x = 0, float V_y = 0, float A_x = 0, float A_y = 0)
    {
        x = X;
        y = Y;
        v_x = V_x;
        v_y = V_y;
        a_x = A_x;
        a_y = A_y;
    }

    bool collision(); /// проверяет на столкновение с взрывом

    void update(float time)
    {
        v_x += a_x * time;
        x += v_x * time;
        v_y += a_y * time;
        y += v_y * time;
        a_x = a_y = 0;
        rocket_body.setPosition(x, y);
        if (collision()) gameOver = true;
    }

    void Draw()
    {
         rocket_PRO_body.setFillColor(Color::Blue);
         window.draw(rocket_body);
    }


};

bool rocket::collision()
{
    float r = (x - b.x)*(x - b.x) + (y- b.y)*(y - b.y);
    if (r < b.radius_wave * b.radius_wave) return true;
    return false;
}

rocket r(50, 250, speed, 0, 0, 0);

class rocket_PRO
{
public:
    float x, y; /// координаты
    float v_x, v_y; /// скорость
    float a_x, a_y; /// ускорение

    rocket_PRO(float X = Width-rocket_size, float Y = ground-rocket_size,
           float V_x = 0, float V_y = 0, float A_x = 0, float A_y = 0)
    {
        x = X;
        y = Y;
        v_x = V_x;
        v_y = V_y;
        a_x = A_x;
        a_y = A_y;
    }

    void update(float time)
    {
        v_x += a_x * time;
        x += v_x * time;
        v_y += a_y * time;
        y += v_y * time;
        a_x = a_y = 0;
        rocket_PRO_body.setPosition(x, y);
    }
    /// отвечает за вырисовывание(здесь же можно добавлять красивые текстуры и т.д., короче, украшать графику)
    void Draw()
    {
            rocket_PRO_body.setFillColor(Color::Red);
            window.draw(rocket_PRO_body);
    }

    void AI();
};

void rocket_PRO::AI()
{
    float distance = sqrt((r.x-x)*(r.x-x) + (r.y-y)*(r.y-y));

    if (distance < min_distance)
    {
        bang = true;
        rocket_launch = false;
        b.x = x;
        b.y = y;
        gameOver = true;
        return;
    }

    if (r.y != y)
    {
        float alpha = (r.x-x)/(r.y-y);
        float c = r.v_x - alpha * r.v_y;
        float v_x_ideal, v_y_ideal;
        v_y_ideal = (alpha * v_x + v_y - c*alpha)/(1+alpha*alpha);
        v_x_ideal = alpha * v_y_ideal + c;

       // std::cout << v_x_ideal << " " << v_y_ideal << " " << alpha << " " <<
       // c << "\n";

        if (abs(v_y_ideal-v_y) <= a_max * dt)
        {
            v_y = v_y_ideal;
        }
        else
        {
            v_y += a_max * sgn(v_y_ideal-v_y) * dt;
        }

        if (abs(v_x_ideal-v_x) <= a_max * dt)
        {
            v_x = v_x_ideal;
        }
        else
        {
            v_x += a_max * sgn(v_x_ideal-v_x) * dt;
        }
    }
    else if (r.x != x)
    {
        if (abs(r.v_y-v_y) <= a_max * dt)
        {
            v_y = r.v_y;
        }
        else
        {
            v_y += a_max * sgn(r.v_y - v_y) * dt;
        }

        if (r.x > x)
        {
            v_x += a_max * dt;
        }
        else
        {
            v_y -= a_max * dt;
        }
    } else gameOver = true;

}

void Draw() /// прорисовка земли
{
    CircleShape c(2);
    for (int i = 0; i < 850; i++)
    {
        c.setPosition(5*i, ground);
        window.draw(c);
    }
}

int main()
{

float deg = 0;
float step = 0.01;

int help = 0;

rocket_PRO PRO(50);

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

    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        (PRO.a_y) -= a_max * 0.1;
    } else
    if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        PRO.a_y += a_max * 0.1;
    } else
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        PRO.a_x -= a_max * 0.1;
    } else
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        PRO.a_x += a_max * 0.1;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Space))
    {
        if (!rocket_launch && !bang)
        {
            rocket_launch = true;
            PRO.v_x = 0;
            PRO.v_y = -0.03;
        }
    }
    else if (Keyboard::isKeyPressed(Keyboard::K))
    {
        help = (help + 1) % 2;
        if (help) window.setTitle("AI");
        else window.setTitle("You");
    }
    else if (Keyboard::isKeyPressed(Keyboard::G))
    {
        PRO.x = 50;
        PRO.y = ground;
        PRO.v_x = PRO.v_y = 0;
    }
   deg  += step;

   // r.v_x = speed * sin(deg);
    r.v_y = speed * cos(deg);

    if (deg >= 360) deg -= 360;

    if (r.x > Width) r.x -= Width/2;

    if (rocket_launch && help) PRO.AI();
     if (rocket_launch) PRO.update(time);
    r.update(time);
    if (bang) b.update(time);
    if (rocket_launch) PRO.Draw();
    r.Draw();
    Draw();   /// рисуем линию земли
    window.display();

    }
window.display();
Sleep(100000);
    return 0;
}
