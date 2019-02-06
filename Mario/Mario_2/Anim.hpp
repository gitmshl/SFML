#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;

class Animation
{
public:
    std::vector<IntRect> frames, flip_frames;
    float current_frame, speed;
    int count_frames;
    bool flip, isPlaying;
    Sprite sprite;

    Animation(Texture &t, int x, int y, int w, int h, int count, float Speed)
    {
        sprite.setTexture(t);
        count_frames = count;
        speed = Speed;
        flip = false;
        isPlaying = true;
        current_frame = 0;

        for (int i = 0; i < count; i++)
        {
            frames.push_back(IntRect(x + i*w, y, w, h));
            flip_frames.push_back(IntRect(x + i*w+w, y, -w, h));
        }
    }

    void tick(float time)
    {
        if (!isPlaying) return;

        current_frame += speed * time;

        if (current_frame > count_frames) current_frame -= count_frames;

        int i = current_frame;

        sprite.setTextureRect(frames[i]);
        if (flip) sprite.setTextureRect(flip_frames[i]);
    }

    void draw(RenderWindow &window, int x = 0, int y = 0)
    {
        sprite.setPosition(x, y);
        window.draw(sprite);
    }
};

struct Map
{
    std::vector<std::string> Names;
    std::vector<Animation> Anim;
    int Size;

    Map()
    {
        Size = 0;
    }

    void Insert(std::string name, Animation anim)
    {
        Names.push_back(name);
        Anim.push_back(anim);
        Size++;
    }

    Animation* get_animation(std::string name)
    {
        for (int i = 0; i <= Size; i++)
            if (Names[i] == name) return &Anim[i];
    }

};

class Animation_Manager
{
public:
    Map animList;
    std::string current_animation;

    Animation_Manager() { }

    void create(std::string name, Texture &t, int x, int y, int w, int h, int count, float Speed)
    {
        Animation a(t, x, y, w, h, count, Speed);
        animList.Insert(name, a);
        current_animation = name;
    }

    void draw(RenderWindow &window, int x = 0, int y = 0)
    {
        Animation *a = animList.get_animation(current_animation);
        a->sprite.setPosition(x, y);
        window.draw(a->sprite);
    }

    void tick(float time)
    {
        Animation *a = animList.get_animation(current_animation);
        a->tick(time);
    }

    void flip(bool flag)
    {
        Animation *a = animList.get_animation(current_animation);
        a->flip = flag;
    }

    void pause()
    {
        Animation *a = animList.get_animation(current_animation);
        a->isPlaying = false;
    }
    void play()
    {
        Animation *a = animList.get_animation(current_animation);
        a->isPlaying = true;
    }

    void set(std::string name)
    {
        current_animation = name;
    }
};


#endif ANIMATION_H
