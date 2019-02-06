#ifndef GRAPHIC_HPP
#define GRAPHIC_HPP

#include "Qvadrocopter.hpp"

#include <cmath>

void Qvadrakopter::draw(float X = 0, float Y = 0)
{
    float W = width/2, H = height/2;
    float x1 = x - (W*cos(angle) - H*sin(angle));
    float y1 = y - (W*sin(angle) + H*cos(angle));
    x1 -= X;
    y1 -= Y;
    r.setPosition(x1, y1);
    float ang = angle * 180.0/pi;
    r.setRotation(ang);
    window.draw(r);
}

void Qvadrakopter::update(float time)
{
    x += v_x * time;
    y += v_y * time;

    if (y > ground)
    {
        y = ground;
        v_y = 0;
        return;
    }

    float F1 = force(w1), F2 = force(w2);
    float ax = ((F1+F2) * sin(angle))/mass;
    float ay = gravity_const - ((F1+F2) * cos(angle))/mass;

    v_x += ax * time;
    v_y += ay * time;

    angle += w * time;

    float epsilon = 3*(F1-F2)/(2 * mass * width);
    w += epsilon * time;

    if (w1 > max_w) w1 = max_w;
    if (w2 > max_w) w2 = max_w;
}

void Qvadrakopter::set_a1_a2(float a1, float a2)
{
    float max_w = 100;
    if (abs(a1) <= max_w) w1 += a1;
    else w1 += sgn(a1) * max_w;

    if (abs(a2) <= max_w) w2 += a2;
    else w2 += sgn(a2) * max_w;
}

void Qvadrakopter::set_w(float w_expect, float time)
{
    if (w_expect == w) return;
    float epsilon = (w_expect - w)/time;
    float a1_expect = sgn(epsilon) * sqrt(2*mass*width*abs(epsilon)/(3*k)) * 0.1;
    //std::cout << "a1_expect " << a1_expect << "\n";
    set_a1_a2(a1_expect, -a1_expect);
}

bool Qvadrakopter::set_angle(float angle_expect, float time)
{
    if (abs(angle_expect - angle) < 0.001) {w = 0; return true; }
    float angle_target = (angle + angle_expect) / 2;
    float w_expect = (angle_target - angle)/time;
    set_w(w_expect, time);
    return false;
}

void Qvadrakopter::set_a(float a_expect)
{
    float w_expect = sqrt(abs(mass * a_expect/k));
    if (w_expect <= max_w) {w1 = w2 = sgn(a_expect) * w_expect; return;}
    w1 = w2 = sgn(a_expect) * max_w;
}

void Qvadrakopter::set_a(float ax, float ay, float time)
{
    if (!ay)
    {
        set_angle(pi/2, time);
        float a_expect = ax;
        set_a(a_expect);
        return;
    }
    float angle_expect = atan(-ax/ay);
    float a_expect;
    if (sin(angle_expect)) a_expect = ax/sin(angle_expect);
    else a_expect = ay/cos(angle_expect);


    //if (ax < 0) std::cout << "a_expect: " << a_expect << "\n";
    set_a(a_expect);
     set_angle(angle_expect, time);
}

void Qvadrakopter::set_v(float v_x_expect, float v_y_expect, float time)
{
    float ax = (v_x_expect - v_x)/time;
    float ay = (v_y_expect - v_y)/time - gravity_const;
    //std::cout << ay << "\n";
    set_a(ax, ay, time);
}

void Qvadrakopter::Move(float x_trg, float y_trg, float time)
{// stabilization(time); return; }
    if (abs(x_trg - x) < width) x_trg = x;
    float coeff = 0.08;
    float v_x_expect = (x_trg - x)/time * coeff;
    float v_y_expect = (y_trg - y)/time * coeff;
    if (abs(x_trg - x) < width/2 && abs(angle) < 0.1 && abs(w) < 10 && abs(v_x) < 0.001) { std::cout << "Move_Up\n"; Move_Up(v_y_expect, time); return;}
   if (abs(x_trg - x) < width/2) v_x_expect = 0;
  //std::cout << v_y_expect << "\n";
    set_v(v_x_expect, v_y_expect, time);
    if (abs(x_trg - x) < width) { std::cout << "stabilization\n"; stabilization(time);}

}

void Qvadrakopter::set_a_y(float a_y_expect)
{
    a_y_expect -= gravity_const;
    set_a(-a_y_expect);
}

void Qvadrakopter::Move_Up(float v_y_expect, float time)
{
    angle = 0;
    w = 0;
    float a_y_expect = (v_y_expect - v_y)/time;
    set_a_y(a_y_expect);
}

void Qvadrakopter::stabilization(float time)
{
     set_angle(0, time);
}

#endif // GRAPHIC_HPP
