#ifndef CONSTS_HPP
#define CONSTS_HPP

#include <SFML/Graphics.hpp>

using namespace sf;

RenderWindow window (VideoMode(800, 510), "PVO");

const float pi = 3.1415926;
const float k = 0.000000001;
const float gravity_const = 0.0000180;
const float ground = 500;
const float max_w = 90; /// максимальная угловая скорость вращения винтов
const float max_change_spin = 0.01; /// максимальное значение, на которое можно изменить
/// спин винта(любого)

float abs(float a)
{
    return a < 0 ? -a : a;
}

float force(float w)
{
   float res = k*w*w;
   return w > 0 ? res : -res;
}

int sgn(float a)
{
    if (!a) return 0;
    return a > 0 ? 1 : -1;
}

#endif // CONSTS_HPP
