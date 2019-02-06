#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <cmath>
#include "Snake\Help.hpp"

using namespace sf;

const int Height = 500;
const int Width = 500;
const int sqr_size = 10;
const int max_snake_size = 2500;
const int radius_food = 4;

bool flag = false;
float step;

float speed = sqr_size;
float x_food, y_food;

bool gameOver = false;

bool Free[51][51];
int count_free = 50*50;

RenderWindow window (VideoMode(Width+sqr_size, Height+sqr_size), "Snakes");
CircleShape food(radius_food);

void rand_food_position(float &x, float &y);

class snake
{
public:
    int snake_size;
    float BodyX[max_snake_size], BodyY[max_snake_size];
    float dx, dy;
    bool Color[51][51];
    snake(float x, float y, float Dx, float Dy)
    {
        BodyX[0] = x;
        BodyY[0] = y;
        dx = Dx;
        dy = Dy;
        snake_size = 1;
    }

    void Move(float time);
    void intersect_food();
    void intersect_wall();
    void intersect();
    void engine_way(int &x_way, int &y_way);
    void rand_food_position(float &x, float &y);
    bool snake_body_position(int x, int y);
    bool snake_intersect(int k, float x_food, float y_food);
    int seek(int x_f, int y_f, int x, int y, int &q);

    void Arr_False()
    {
        for (int i = 0; i < 51; i++)
            for (int j = 0; j < 51; j++)
                Color[i][j] = false;

        for (int i = 0; i < snake_size; i++)
            Color[(int)(BodyX[i]/10)][(int)(BodyY[i]/10)] = true;
    }

    void update(float time)
    {
        Move(time);
    intersect();
       intersect_food();
        intersect_wall();
        RectangleShape rect (Vector2f(sqr_size, sqr_size));

        rect.setFillColor(Color::Red);
        rect.setPosition(BodyX[0], BodyY[0]);
        window.draw(rect);

        for (int i = 1; i < snake_size; i++)
        {
            rect.setFillColor(Color::Blue);
            rect.setPosition(BodyX[i], BodyY[i]);
            window.draw(rect);
        }
    }

    void AI_Snake()
    {
        int x_way, y_way;
       // std::cout << "0\n";
        engine_way(x_way, y_way);
        dx = x_way * speed;
        dy = y_way * speed;
    }

};

int max(int a, int b)
{
    if (a > b) return a;
    return b;
}

int max(int a, int b, int c, int d)
{
    return max(max(a,b), max(c, d));
}


int sgn(float a)
{
    if (a > 0) return 1;
    else if (a < 0) return -1;
    return 0;
}

int way(int dx1, int dy1)
{
    if (dx1 == 1) return 1;
    if (dx1 == -1) return 3;
    if (dy1 == 1) return 2;
    if (dy1 == -1) return 0;
    return -1;
}
void way(int w, int &x_way, int &y_way)
{
    if (w == 0) {x_way = 0; y_way = -1;}
    else if (w == 1) {x_way = 1; y_way = 0;}
    else if (w == 2) {x_way = 0; y_way = 1;}
    else if (w == 3) {x_way = -1; y_way = 0;}
    else {x_way = y_way = 0;}
}

int except_way(int w1, int w2, int w3)
{
    for (int k = 0; k < 4; k++)
    {
        if (k != w1 && k != w2 && k != w3) return k;
    }
}

void snake::engine_way(int &x_way, int &y_way)
{
    int w = way_engine(BodyX, BodyY, snake_size, x_food, y_food);
    way(w, x_way, y_way);
    ///Sleep(100000);
}

void snake::intersect()
{
    FloatRect head(BodyX[0], BodyY[0], sqr_size, sqr_size);
    for (int k = 1; k < snake_size; k++)
    {
        FloatRect body(BodyX[k], BodyY[k], sqr_size, sqr_size);
        if (head.intersects(body)) {gameOver = true; break;}
    }
}

void snake::Move(float time)
{
    float prev_x, prev_y, prev2_x, prev2_y;
    prev_x = BodyX[0];
    prev_y = BodyY[0];
    BodyX[0] += dx;
    BodyY[0] += dy;
    for (int k = 1; k < snake_size; k++)
    {
        prev2_x = BodyX[k];
        prev2_y = BodyY[k];
        BodyX[k] = prev_x;
        BodyY[k] = prev_y;
        prev_x = prev2_x;
        prev_y = prev2_y;
    }
}

void snake::intersect_food()
{
    float x = BodyX[0], y = BodyY[0];
    float p = x_food + radius_food, p0 = x_food - radius_food;
    float q = y_food + radius_food, q0 = y_food - radius_food;

    if (!(q < y || q0 > y + sqr_size))
    {
        if (!(p0 > x + sqr_size || p < x))
        {
            BodyX[snake_size] = BodyY[snake_size] = -10;
            snake_size ++;
            count_free--;
            rand_food_position(x_food, y_food);
            food.setPosition(x_food, y_food);
        }
    }
}



void snake::intersect_wall()
{
    if (BodyX[0] < 0) BodyX[0] = Width;
    else if (BodyX[0] > Width) BodyX[0] = 0;
    else if (BodyY[0] < 0) BodyY[0] = Height;
    else if (BodyY[0] > Height) BodyY[0] = 0;
}

bool snake::snake_intersect(int k, float x_food, float y_food)
{
    float x = BodyX[k], y = BodyY[k];
    float p = x_food + radius_food, p0 = x_food - radius_food;
    float q = y_food + radius_food, q0 = y_food - radius_food;

    if (!(q < y || q0 > y + sqr_size))
    {
        if (!(p0 > x + sqr_size || p < x))
        {
            return true;
        }
    }
    return false;
}

bool snake::snake_body_position(int x, int y)
{
    for (int i = 0; i < snake_size; i++)
    {
        if (BodyX[i] == x && BodyY[i] == y) return true;
    }

    return false;
}

void snake::rand_food_position(float &x, float &y)
{
   step = rand()%count_free+1;
   int q = 0;
   for (int i = 0;  i < 51; i++)
    for (int j = 0; j < 51; j++)
        if (!snake_body_position(10*i, 10*j))
        {
            q++;
            if (q == step)
            {
                x = i*10+5;
                y = j*10+5;
            }
        }
}

int main()
{

    snake s(50, 150, speed, 0);

    food.setFillColor(Color::Green);

    x_food = y_food = 100;
    food.setPosition(x_food, y_food);

    Clock clock;

    int help = 0;
    int counter = 0;
    int border = 3;
    int time_time = 0;

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

        if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        if (s.dy != speed)
        {
            s.dx = 0; s.dy = -speed;
        }
    } else
    if (Keyboard::isKeyPressed(Keyboard::Down))
    {
        if (s.dy != -speed)
        {
            s.dx = 0; s.dy = speed;
        }
    } else
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        if (s.dx != speed)
        {
            s.dx = -speed; s.dy = 0;
        }
    } else
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
         if (s.dx != -speed)
        {
            s.dx = speed; s.dy = 0;
        }
    }

    if (Keyboard::isKeyPressed(Keyboard::Space))
    {
       if (counter > border)
       {
           help = (help + 1) % 2;
           counter = 0;
           if (help) window.setTitle("AI");
           else window.setTitle("You");
       }
       counter ++;
    }

    if (Keyboard::isKeyPressed(Keyboard::K))
    {
        time_time = (time_time + 1) % 2;
    }
    window.clear();
 s.AI_Snake();

   s.update(time);

Sleep(30);
    window.draw(food);
    window.display();
    }
window.display();
Sleep(100000);
    return 0;
}
