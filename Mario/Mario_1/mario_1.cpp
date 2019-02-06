/// Марио 1.0 6.02.18 4:23
#include <SFML/Graphics.hpp>
#include <windows.h>

using namespace sf;

const int H = 17;
const int W = 150;
const int Width = 500;
const int Height = 500;
const int ground = Height-60;
const int square_size = 10;
const int deltaY = Height-180;
const int enemy_size = 13;

float offsetX = 0, offsetY = 0;
int enemy_start_x = 30;
int enemy_start_y = 12;

String TileMap[H] = {
"000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000",
"0                                                                                                                                                    0",
"0                                                                                    w                                                               0",
"0                   w                                  w                   w                                                                         0",
"0                                      w                                       kk                                                                    0",
"0                                                                             k  k    k    k                                                         0",
"0                      c                                                      k      kkk  kkk  w                                                     0",
"0                                                                       r     k       k    k                                                         0",
"0                                                                      rr     k  k                                                                   0",
"0                                                                     rrr      kk                                                                    0",
"0               c    kckck                                           rrrr                                                                            0",
"0                                      t0                           rrrrr                                                                            0",
"0G                                     00              t0          rrrrrr            G                                                               0",
"0           d    g       d             00              00         rrrrrrr                                                                            0",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
"PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP",
};


class player
{
public:
   FloatRect rect;
    float dx, dy;
    bool onGround;
    Sprite sprite;
    float current_Frame;
    player(Texture &t)
    {
        rect = FloatRect(80, ground, 40, 50);
        sprite.setTexture(t);
        sprite.setTextureRect(IntRect(0, 244, 40, 50));
        dx = dy = 0;
        onGround = true;
        current_Frame = 0;
    }
   void CollisionX(bool flag);
   void CollisionY();
   void Gravity();
    void update(float time)
    {
        rect.left += dx*time;
      if (dx*time) CollisionX(false);
        if (!onGround) dy += 0.0005*time;
        rect.top += dy*time;
        if (dy*time) CollisionY();

    Gravity();

        if (rect.top > ground) {rect.top = ground; dy = 0; onGround = true;}

        current_Frame += 0.005*time;
        if (current_Frame >= 6) current_Frame -= 6;
    if (dx > 0)  sprite.setTextureRect(IntRect(40*int(current_Frame), 244, 40, 50));
    if (dx < 0) sprite.setTextureRect(IntRect(40*int(current_Frame)+40, 244, -40, 50));
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
        dx = 0;
    }
};

class Enemy : public player
{
public:
    bool life;
    Enemy(Texture &image, int x, int y) : player(image)
    {
        sprite.setTextureRect(IntRect(0, 0, enemy_size, enemy_size));
        rect = FloatRect(x, y, enemy_size, enemy_size);
        dx = 0.05;
        life = true;
    }
    void update(float time)
    {
        rect.left += dx*time;
        CollisionX(true);

        current_Frame += 0.005*time;
        if (current_Frame > 2) current_Frame -= 2;
        sprite.setTextureRect(IntRect(18*int(current_Frame), 0, enemy_size, enemy_size));

        if (!life) sprite.setTextureRect(IntRect(18, 0, 0, 0));
        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);
    }
};

void player::CollisionX(bool flag)
{
    int p0 = (rect.top-deltaY)/square_size;
    int p = (rect.top-deltaY+rect.height)/square_size;
    bool Flag = false;
    for (int i = p0; i < p; i++)
    {
        for (int j = rect.left/square_size; j < (rect.left+rect.width)/square_size; j++)
        {
        if (i >= 0 && i < H && j >= 0 && j < W)
        {
           if (TileMap[i][j] == 'P')
            {
                if (dx > 0) {rect.left = j*square_size-rect.width; Flag = true; }
                if (dx < 0) {rect.left = j*square_size+square_size; Flag = true; }
            }
            else if (TileMap[i][j] == 'c') TileMap[i][j] = ' ';
        }
        }
    }
    if (flag && Flag) dx *= -1;
}

void player::CollisionY()
{
    int p0 = (rect.top-deltaY)/square_size;
    int p = (rect.top-deltaY+rect.height)/square_size;
    for (int i = p0; i < p; i++)
    {
        for (int j = rect.left/square_size; j < (rect.left+rect.width)/square_size; j++)
        {
        if (i >= 0 && i < H && j >= 0 && j < W)
            {
                if (TileMap[i][j] == 'P')
            {
                if (dy > 0) {rect.top = i*square_size+deltaY-rect.height; dy = 0;onGround = true; }
                if (dy < 0) {rect.top = i*square_size+deltaY+square_size; dy = 0; }
            }
            }
        }
    }
}

void player::Gravity()
{
    int p = (rect.top-deltaY+rect.height)/square_size;
    bool flag = false;
    for (int q = rect.left/square_size; q <= (rect.left+rect.width)/square_size; q++)
    {
         if (p >= 0 && p < H && q >= 0 && q < W)
         {
             if (TileMap[p][q] == 'P') {flag = true; break;}
         }
    }
    if (!flag) onGround = false;
    else onGround = true;
}

int main()
{
RenderWindow window(VideoMode(Width, Height), "Mario 1.0");
    Texture t, tileset;
    t.loadFromFile("fang.png");
    player p(t);
    tileset.loadFromFile("Mario_tileset.png");
    Enemy enemy(tileset, enemy_start_x*square_size, enemy_start_y*square_size + deltaY+5);

    RectangleShape rectangle(Vector2f(square_size,square_size));

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
    if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        p.dx = -0.1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        p.dx = 0.1;
    }
    if (Keyboard::isKeyPressed(Keyboard::Up))
    {
        if (p.onGround)
        {
        p.dy = -0.3;
        p.onGround = false;
        }
    }

 //std::cout << p.rect.left << " " << p.rect.top << " ; " << enemy.rect.left << " " << enemy.rect.top << "\n";

 if (p.rect.intersects(enemy.rect))
 {
     if (p.dy > 0) enemy.life = false;
     else if (enemy.life) p.sprite.setColor(Color::Red);
 }

if (p.rect.left > Width/2 && p.rect.left < W*square_size-Width/2) offsetX = p.rect.left-Width/2;
 //offsetY = p.rect.top-Height/2;

p.update(time);
 enemy.update(time);

    window.clear();
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            if (TileMap[i][j] == 'P') {rectangle.setFillColor(Color::Red); }
            else if (TileMap[i][j] == 'k') {rectangle.setFillColor(Color::Blue);}
            else if (TileMap[i][j] == 'c') rectangle.setFillColor(Color::White);
            else if (TileMap[i][j] == 'w') rectangle.setFillColor(Color::Yellow);
            else if (TileMap[i][j] == 'r') {rectangle.setFillColor(Color::Magenta); TileMap[i][j] = 'c';}
            else if (TileMap[i][j] == 't') {rectangle.setFillColor(Color::Cyan); TileMap[i][j] = 'c';}
            else if (TileMap[i][j] == 'g') rectangle.setFillColor(Color::Transparent);
            else if (TileMap[i][j] == 'G') rectangle.setFillColor(Color::Blue);
            else if (TileMap[i][j] == 'd') rectangle.setFillColor(Color::Yellow);
            else if (TileMap[i][j] == '0') {rectangle.setFillColor(Color::Green); TileMap[i][j] = 'P';}
            else continue;

            rectangle.setPosition(j*square_size - offsetX, i*square_size+deltaY-offsetY);
            window.draw(rectangle);
        }
    }

    window.draw(p.sprite);
    window.draw(enemy.sprite);
    window.display();
    }

    return 0;
}
