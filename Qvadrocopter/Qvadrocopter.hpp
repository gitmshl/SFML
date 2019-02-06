#ifndef QVADROCOPTER_HPP
#define QVADROCOPTER_HPP

#include "Consts.hpp"

using namespace sf;

class Qvadrakopter
{
public:
    float x, y; /// координаты центра
    float v_x, v_y; /// скорости по осям
    float angle; /// угол к горизонтали
    float w; /// угловая скорость вращения
    float w1, w2; /// угловая скорость вращения вентилей
    float mass; /// масса квадракоптера
    float width, height;
    RectangleShape r;

    Qvadrakopter(float X = 100, float Y = ground, float Mass = 1)
    {
        x = X; y = Y;
        v_x = v_y = 0;
        angle = 0;
        w1 = w2 = 94.9;
        w = 0;
        mass = Mass;
        width = 20; height = 5;
        r.setSize(Vector2f(width, height));
    }

    void draw(float X, float Y); /// сдвиги
    void update(float time);
    void set_a1_a2(float a1, float a2);
    void set_w(float w_expect, float time);
    bool set_angle(float angle_expect, float time);
    void set_a(float a_expect);
    void set_a(float ax, float ay, float time);
    void set_a_y(float a_y_expect);
    void set_v(float v_x_expect, float v_y_expect, float time);
    void Move(float x_trg, float y_trg, float time);
    void Move_Up(float v_y_expect, float time);
    void stabilization(float time);
};

#endif // QVADROCOPTER_HPP
