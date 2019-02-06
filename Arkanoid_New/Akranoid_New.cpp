#include <SFML\Graphics.hpp>
#include <ctime>
#include <iostream>
#include <windows.h>
#include <cmath>

using namespace sf;

const int Width = 900;
const int Height = 650;

const int width_standart = 80;
const int height_standart = 10;

const int enemy_width_min = 10;

const int ball_radius = 5;

const int count_levels = 1;

const double speed = 0.15;

double SUMMER_SPEED;

bool gameOver = false;

int enemy_count = 0; /// следит за числом врагов

int sgn(double a)
{
    if (a > 0) return 1;
    if (a < 0) return -1;
    return 0;
}

double abs(double a)
{
    if (a < 0) return -a;
    return a;
}

RenderWindow window (VideoMode(Width, Height), "Arkanoid");

class Ball
{
public:
    CircleShape Ball1;
    bool Collision;
    double x, y, dx, dy;

    Ball(float X, float Y, float Dx, float Dy, Color color = Color::Red)
    {
        x = X;
        y = Y;
        Ball1 = CircleShape(ball_radius);
        Ball1.setPosition(x, y);
        Ball1.setFillColor(color);
        dx = Dx; dy = Dy;
    }

    void change_speed(bool vertical) /// производит изменение вектора скорости мяча при столкновении
    {
        std::cout <<"Before: " << dx << " " << dy << "\n";
        int a = rand()%10;

        if (a == 0) {dx *= -1; dy *= -1;}
        else
        {
            if (vertical)
            {
                dy *= -1;
                double b = (rand()%10 - 5)/100.0;
                dy += b;
                double c = SUMMER_SPEED-dy*dy;
                if (c >= 0)
                    dx = sgn(dx) * sqrt(c);
                else dy -= b;
            }
            else
            {
                dx *= -1;
                double b = (rand()%10 - 5)/100.0;
                dx += b;
                 double c = SUMMER_SPEED-dx*dx;
                if (c >= 0)
                    dy = sgn(dy) * sqrt(c);
                else dx -= b;
            }
        }
    std::cout <<"After: " << dx << " " << dy << "\n";
    std::cout << "Summer_speed = " << SUMMER_SPEED << "\n";
    }

    void collision() /// проверка на столкновение со стенами
    {
        if (!Collision) /// чтобы не было курьезных ситуаций
        {
            if (y - ball_radius < 0)
                {y = ball_radius; Collision = true; change_speed(true); }
            else if (x - ball_radius < 0)
            { x = ball_radius; Collision = true; change_speed(false);}
            else if (x + ball_radius > Width)
                { x = Width - ball_radius; Collision = true; change_speed(false);}
        }
    }

    void Draw()
    {
        window.draw(Ball1);
    }
    void update(float time)
    {
        x += dx * time;
        if (y > Height) gameOver = true;
        y += dy * time;

        collision();

        Ball1.setPosition(x, y);
        if (enemy_count == 0) gameOver = true;
    }
} ball(Width/2, Height-100, 0.08, -0.09);


class Enemy
{
public:
    RectangleShape rect;
    double x, y;
    double dx;
    int width, height;
    bool dead;

    Enemy(double X, double Y, int Width = width_standart, int Height =
          height_standart, double Dx = 0)
    {
        rect.setSize(Vector2f(Width, Height));
        x = X; y = Y; height = Height; width = Width;
        dx = Dx;
        rect.setPosition(x, y);
        int a = rand()%7;
        switch (a)
        {
            case 0: {rect.setFillColor(Color::Black); break;}
            case 1: {rect.setFillColor(Color::Blue); break;}
            case 2: {rect.setFillColor(Color::Green); break;}
            case 3: {rect.setFillColor(Color::Magenta); break;}
            case 4: {rect.setFillColor(Color::Red); break;}
            case 5: {rect.setFillColor(Color::White); break;}
            case 6: {rect.setFillColor(Color::Yellow); break;}
        }
        dead = false; /// не мертв (при создании)
    }
    virtual void update(float time){}

    void Draw()
    {
        if (!dead)
        {
            rect.setPosition(x, y);
            window.draw(rect);
        }
    }
    void collision()
    {
        if (!ball.Collision)
        {
    double p0 = ball.x - ball_radius;
    double p1 = ball.x + ball_radius;
    double q0 = ball.y - ball_radius;
    double q = ball.y + ball_radius;
/// для player
    if (p0 < x + width && p1 > x + width)
    {
        if (!((q < y) || (q0 > y + height)))
        {
            ball.x = x + width + ball_radius;
            ball.change_speed(false);
            dead = true;
        }
    }
    else if (p0 < x + width && p1 > x)
    {
        if ((q > y && q0 < y) )
        {
            ball.y = y - ball_radius;
            ball.change_speed(true);
            dead = true;
        }
        else if (q > y + height && q0 < y + height)
        {
            ball.y = y + height + ball_radius;
            ball.change_speed(true);
            dead = true;
        }
    }

        }
    }
};


class Player : public Enemy
{
public:
    Player(double X, double Y, int Width = width_standart, int Height =
          height_standart, double Dx = 0) :
              Enemy(X, Y, Width, Height, Dx) {}
     void update(float time)
    {
        x += dx * time;
        if (x < 0) {x = 0; dx = 0;}
        else if (x + width > Width) {x = Width - width; dx = 0;}
        rect.setPosition(x, y);
        dx = 0;
    }

      double approcsimation();

    void AI_Enemy()
    {
        double X;
     if (ball.dy > 0)
     {
            X = approcsimation();
            if ((x + width/2 - 10 < X) && (x + width/2 + 10 > X)) dx = 0;
            else dx = sgn(X-x) * speed;
     }
    else
    {
        X = Width/2;
           if ((x + width/2 - 10 < X) && (x + width/2 + 10 > X)) dx = 0;
            else dx = sgn(X-x) * speed;
    }

    }
};

double Player::approcsimation()
{
  double alpha = ball.dx/ball.dy;
  double X;
  double H = Height - 10 - height_standart;
  X = ball.x + alpha * (H - ball.y);
  if (X > 0 && X < Width) return X;

  if (X < 0)
  {
      return -X;
  }
  else
  {
      return (2*Width - X);
  }
}

int generation() /// генерирует длину прямоугольника
{
    return width_standart;
}

int main()
{
    SUMMER_SPEED = ball.dx * ball.dx + ball.dy * ball.dy;
    /// заполняем массив врагов
    int current_level = 1, cursor_current_level;
    Enemy * Enemys[(Width/width_standart+1)*count_levels];

    for ( ; current_level <= count_levels; current_level++)
    {
        cursor_current_level = 0;
        while (cursor_current_level < Width)
        {
            int width = generation();

            Enemys[enemy_count++] = new  Enemy (cursor_current_level, (current_level-1)*height_standart, width_standart, height_standart);
            cursor_current_level += width;
        }
    }



Player p(Width/2, Height - 10 - height_standart, width_standart + 30);

bool help = true;

p.rect.setFillColor(Color::Blue);

 Clock clock;
    /// Пишем процесс игры
while (window.isOpen() && !gameOver)
    {
        ball.Collision = false;

       float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 360;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


     if (Keyboard::isKeyPressed(Keyboard::Left))
     {
         p.dx = -speed;
     }
     if (Keyboard::isKeyPressed(Keyboard::Right))
     {
          p.dx = speed;
     }
    if (Keyboard::isKeyPressed(Keyboard::K))
     {
            if (help) help = false;
            else help = true;
     }

    p.collision();
    for (int i = 0; i < enemy_count; i++)
        if (!(Enemys[i]->dead)) Enemys[i]->collision();
    ball.collision();

    p.dead = false;

    if (help) p.AI_Enemy();

    p.update(time);
    ball.update(time);

    window.clear(Color::Cyan); /// Magenta - фиолетовый

    p.Draw();
    for (int i = 0; i < enemy_count; i++)
      {
         Enemys[i]->Draw();
      }
    ball.Draw();
SUMMER_SPEED = ball.dx * ball.dx + ball.dy * ball.dy;
    window.display();
    }

    std::cout << enemy_count;
    return 0;
}
