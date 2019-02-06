#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

const int Height = 500;
const int Width = 500;
const float k_height = 10;
const float k_width = 30;
const float radius_ball = 5;

bool gameOver;
float x_ball, y_ball;
float dx_ball, dy_ball;
float speed = 0.1;
float ball_speed;

enum Levels {easy = 0, middle, hard};

Levels level;

int sgn(float a)
{
    if (a > 0) return 1;
    else if (a < 0) return -1;
    return 0;
}

class player
{
public:
    RectangleShape rect;
    float x, y;
    float width, height;
    float dy;
    bool smart;
    float x_AI_Engine;

    player(float x1, float y1, Color color)
    {
        rect.setSize(Vector2f(Width/k_width, Height/k_height));
        rect.setPosition(x1, y1);
        x = x1; y = y1;
        width = Width/k_width;
        height = Height/k_height;
        rect.setFillColor(color);
        dy = 0;
    }

    void update(float time)
    {
        y += dy * time;
        if (y < 0) {y = 0; dy = 0;}
        else if (y + height > Height) {y = Height - height; dy = 0;}
        rect.setPosition(x, y);
        dy = 0;
    }

    float approcsimation();

    void AI_player()
    {
     if (x_ball <= x_AI_Engine && dx_ball < 0)
     {
            float Y = approcsimation();
            if (y < Y  && y + height > Y ) dy = 0;
            else dy = sgn(Y-y) * speed;
     }
     else if (smart)
     {
         float Y = Height / 2;
         if (y < Y && y + height > Y) dy = 0;
            else dy = sgn(Y-y) * speed;
     }
    }
};

class Enemy
{
public:
    RectangleShape rect;
    float x, y;
    float width, height;
    float dy;
    bool smart;
    float x_AI_Engine;

    Enemy(float x1, float y1, Color color)
    {
        rect.setSize(Vector2f(Width/k_width, Height/k_height));
        rect.setPosition(x1, y1);
        x = x1; y = y1;
        width = Width/k_width;
        height = Height/k_height;
        rect.setFillColor(color);
        dy = 0;
    }

    float approcsimation();

    void update(float time)
    {
        y += dy * time;
        if (y < 0) {y = 0; dy = 0;}
        else if (y + height > Height) {y = Height - height; dy = 0;}
        rect.setPosition(x, y);
        dy = 0;
    }

    void AI_Enemy()
    {
     if (x_ball >= x_AI_Engine && dx_ball > 0)
     {
            float Y = approcsimation();
            if (y < Y && y + height > Y) dy = 0;
            else dy = sgn(Y-y) * speed;
     }
     else if (smart)
     {
         float Y = Height / 2;
         if (y < Y && y + height > Y) dy = 0;
            else dy = sgn(Y-y) * speed;
     }
    }
};

player p(20, Height/2, Color::Magenta);
Enemy enemy(Width - 20, Height/2, Color::Magenta);

float player::approcsimation()
{
    float tx, ty, Y, H;
    H = Height;
    tx = (x_ball-20-p.width)/((-1)*dx_ball);

    if (dy_ball > 0) ty = (H-y_ball)/dy_ball;
    else if (dy_ball < 0)
    {
        ty = y_ball/((-1) * dy_ball);
    }
    else ty= 99999; /// î÷åíü áîëüøîå ÷èñëî

    Y = y_ball + dy_ball * tx;
    if (smart && ty < tx)
    {
       if (dy_ball > 0) Y = 2*H-Y;
       else if (dy_ball < 0) Y = -Y;
    }
    return Y;
}

float Enemy::approcsimation()
{
    float tx, ty, Y, W, H;
    W = x;
    H = Height;
    tx = (W-x_ball)/dx_ball;
    if (dy_ball > 0) ty = (H-y_ball)/dy_ball;
    else if (dy_ball < 0)
    {
        ty = y_ball/((-1) * dy_ball);
    }
    else ty= 99999; /// î÷åíü áîëüøîå ÷èñëî

    Y = y_ball + dy_ball * tx;
    if (smart && ty < tx)
    {
       if (dy_ball > 0) Y = 2*H-Y;
       else if (dy_ball < 0) Y = -Y;
    }
    return Y;
}

class Ball
{
public:
    CircleShape ball;
    float dx, dy;

    Ball(float x, float y, float Dx, float Dy, Color color)
    {
        x_ball = x;
        y_ball = y;
        ball = CircleShape(radius_ball);
        ball.setPosition(x, y);
        ball.setFillColor(color);
        dx = Dx; dy = Dy;
    }
    void CollisionX();
    void CollisionY();

    void update(float time)
    {
        x_ball += dx * time;
        if (x_ball < 0 || x_ball > Width) gameOver = true;
        CollisionX();

        y_ball += dy * time;
        CollisionY();

        ball.setPosition(x_ball, y_ball);
    }
};

void Ball::CollisionX()
{
    float p0 = x_ball - radius_ball;
    float p1 = x_ball + radius_ball;
    float q0 = y_ball - radius_ball;
    float q = y_ball + radius_ball;
/// äëÿ player
    if (p0 < p.x + p.width && p1 > p.x + p.width)
    {
        if (!((q < p.y) || (q0 > p.y + p.height)))
        {
            dx *= -1;
            x_ball = p.x + p.width + radius_ball;
        }
    }
    else if (p0 < p.x + p.width && p1 > p.x)
    {
        if ((q > p.y && q0 < p.y) )
        {
            dy *= -1;
            y_ball = p.y - radius_ball;
        }
        else if (q > p.y + p.height && q0 < p.y + p.height)
        {
            dy *= -1;
            y_ball = p.y + p.height + radius_ball;
        }
    }

   /// Äëÿ Enemy
    if (p1 > enemy.x && p0 < enemy.x)
    {
        if (!((q < enemy.y) || (q0 > enemy.y + enemy.height)))
        {
            dx *= -1;
            x_ball = enemy.x - radius_ball;
        }
    }
    else if (p1 > enemy.x && p0 > enemy.x + enemy.width)
    {
        if (q > enemy.y && q0 < enemy.y)
        {
            dy *= -1;
            y_ball = enemy.y - radius_ball;
        }
        else if (q > enemy.y + enemy.height && q0 < enemy.y + enemy.height)
        {
            dy *= -1;
            y_ball = enemy.y + enemy.height + radius_ball;
        }
    }
}

void Ball::CollisionY()
{
    float q0 = y_ball - radius_ball;
    float q = y_ball + radius_ball;

    if (q0 < 0) {dy *= -1; y_ball = radius_ball; }
    else if (q > Height) {dy *= -1; y_ball = Height - radius_ball;}
}

float rand_speed(float g) /// g - ãðàíèöà ( <= 1)
{
    int d = g*1000;
    d = 2 * (rand()%d);
    d -= 1000*g;
    //std::cout << ": " << d << " " << 1000*g << "\n\n\n";
    float f = d/(1000.0);
    return f;
}

int main()
{
    gameOver = false;

    level = hard;

    RenderWindow window (VideoMode(Width, Height), "Arkanoid");
    float dx, dy;

    if (level == easy) ball_speed = 0.1;
    else if (level == middle) ball_speed = 0.12;
    else if (level == hard) ball_speed = 0.15;

    if (level == easy) {enemy.smart = false; p.smart = false;}
        else {enemy.smart = true; p.smart = true;}
        if (level == easy) {enemy.x_AI_Engine = enemy.x/2; p.x_AI_Engine = enemy.x/2;}
        else if (level == middle) {enemy.x_AI_Engine = enemy.x/2 - 20; p.x_AI_Engine = enemy.x/2 + 20;}
        else {enemy.x_AI_Engine = 0; p.x_AI_Engine = Width;}

    dx = rand_speed(ball_speed);
    dy = rand_speed(ball_speed);
    //std::cout << dx << " " << dy << "\n";

    Ball ball(Width/2, Height/2, dx, dy, Color::Red);

    Clock clock;
    enemy.dy = -0.1;
    while (window.isOpen() && !gameOver)
    {
        dx_ball = ball.dx;
        dy_ball = ball.dy;

       float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time /= 360;

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

     if (Keyboard::isKeyPressed(Keyboard::Up))
     {
         p.dy = -speed;
     }
     if (Keyboard::isKeyPressed(Keyboard::Down))
     {
          p.dy = speed;
     }

    p.AI_player();
    p.update(time);
   enemy.AI_Enemy();
    enemy.update(time);

    ball.update(time);
       window.clear(Color::Cyan); /// Magenta - ôèîëåòîâûé

        window.draw(p.rect);
        window.draw(enemy.rect);
    window.draw(ball.ball);
        window.display();
    }

    return 0;
}
