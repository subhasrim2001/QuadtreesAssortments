#include<iostream>
#include<vector>
#include<math.h>
#include<sstream>
#include<string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#define SCREEN_W 600
#define SCREEN_H 600
#define LEVEL_MAX 4
#define CAPACITY 5 //efficiency changes with capacity
#define PI 3.14159265
#define SPEED 0.5f

string intToString(int num)
{
    static stringstream toStringConverter;
    static string tempString;
    toStringConverter.clear();
    toStringConverter << num;
    toStringConverter >> tempString;
    //take num and convert it to string and store in tempString
    return tempString;
}

class Rectangle
{
    public:
            float x;
            float y;
            float w;
            float h;
            Rectangle(float x, float y, float w, float h)
            {
                this->x = x;
                this->y = y;
                this->w = w;
                this->h = h;
            }
            bool intersects(const Rectangle &other) const
            {
                return !(x - w > other.x + other.w || x + w < other.x - other.w || y - h > other.y + other.h || y + h < other.y - other.h);
            }
            void draw(RenderTarget &t)
            {
                static Vertex vertices[5];
                vertices[0] = Vertex(Vector2f(x - w, y - h), Color::Magenta);
                vertices[1] = Vertex(Vector2f(x + w, y - h), Color::Magenta);
                vertices[2] = Vertex(Vector2f(x - w, y + h), Color::Magenta);
                vertices[3] = Vertex(Vector2f(x + w, y + h), Color::Magenta);
                vertices[4] = Vertex(Vector2f(x - w, y - h), Color::Magenta);
                t.draw(vertices, 5, LineStrip);
            }
};

class Entity : public Rectangle
{
    public:
            bool collides;
            float angle;
            Entity(float x, float y, float w, float h) : Rectangle(x, y, w, h)
            {
                collides = false;
                angle = 0;
            }
            void Move()
            {
                x += cos(angle / 180 * PI) * SPEED;
                y += sin(angle / 180 * PI) * SPEED;
                if(x < w)
                {
                    x = w;
                    angle = 180 - angle;
                    angle += rand() % 21 - 10;
                }
                else if(x > SCREEN_W - w)
                {
                    x = SCREEN_W - w;
                    angle = 180 - angle;
                    angle += rand() % 21 - 10;
                }
                if(y < h)
                {
                    y = h;
                    angle = -angle;
                    angle += rand() % 21 - 10;
                }
                else if(y > SCREEN_H - h)
                {
                    y = SCREEN_H - h;
                    angle = -angle;
                    angle += rand() % 21 - 10;
                }
            }

};
class QuadTree {
    private:
            QuadTree *NorthWest;
            QuadTree *NorthEast;
            QuadTree *SouthWest;
            QuadTree *SouthEast;
            Rectangle boundaries;
            bool divided;
            size_t capacity;
            size_t level;
            vector<Entity*> children;
            void subdivide()
            {
                static Vector2f halfSize;
                halfSize.x = boundaries.w / 2.0f;
                halfSize.y = boundaries.h / 2.0f;
                NorthWest = new QuadTree(Rectangle(boundaries.x - halfSize.x, boundaries.y - halfSize.y, halfSize.x, halfSize.y), capacity, level + 1);
                NorthEast = new QuadTree(Rectangle(boundaries.x + halfSize.x, boundaries.y - halfSize.y, halfSize.x, halfSize.y), capacity, level + 1);
                SouthWest = new QuadTree(Rectangle(boundaries.x - halfSize.x, boundaries.y + halfSize.y, halfSize.x, halfSize.y), capacity, level + 1);
                SouthEast = new QuadTree(Rectangle(boundaries.x + halfSize.x, boundaries.y + halfSize.y, halfSize.x, halfSize.y), capacity, level + 1);
                divided = true;
            }
    public:
            QuadTree(const Rectangle &_boundaries, size_t capacity, size_t level) : boundaries(_boundaries)
            {
                NorthWest = NULL;
                NorthEast = NULL;
                SouthWest = NULL;
                SouthEast = NULL;
                divided = false;
                this->capacity = capacity;
                this->level = level;
                if(this->level >= LEVEL_MAX)
                {
                    this->capacity = 0;
                }
            }
            ~QuadTree()
            {
                if(divided)
                {
                    delete NorthWest;
                    delete NorthEast;
                    delete SouthWest;
                    delete SouthEast;
                }
            }
            void query(const Rectangle &area, vector<Entity*> &found) const
            {
                if(!area.intersects(boundaries))
                {
                    return;
                }
                if(divided)
                {
                    NorthWest->query(area, found);
                    NorthEast->query(area, found);
                    SouthWest->query(area, found);
                    SouthEast->query(area, found);
                }
                else
                {
                    for(size_t i = 0; i < children.size(); ++i)
                    {
                        if(area.intersects(*children[i]))
                        {
                            found.push_back(children[i]);
                        }
                    }
                }
            }
            void insert(Entity *e)
            {
                if(!boundaries.intersects(*e))
                {
                    return;
                }
                if(!divided)
                {
                    children.push_back(e);
                    if(children.size() > capacity && capacity != 0 )
                    {
                        subdivide();
                        vector<Entity*>::iterator it = children.begin();
                        while(it != children.end())
                        {
                            NorthWest->insert(*it);
                            NorthEast->insert(*it);
                            SouthWest->insert(*it);
                            SouthEast->insert(*it);
                            it = children.erase(it);
                        }
                    }
                }
                else
                {
                    NorthWest->insert(e);
                    NorthEast->insert(e);
                    SouthWest->insert(e);
                    SouthEast->insert(e);
                }
            }

            void draw(RenderTarget &t)
            {

                if(divided)
                {
                    static Vertex vertices[4];
                    vertices[0] = Vertex(Vector2f(boundaries.x, boundaries.y - boundaries.h), Color::White);
                    vertices[1] = Vertex(Vector2f(boundaries.x, boundaries.y + boundaries.h), Color::White);
                    vertices[2] = Vertex(Vector2f(boundaries.x - boundaries.w, boundaries.y), Color::White);
                    vertices[3] = Vertex(Vector2f(boundaries.x + boundaries.w, boundaries.y), Color::White);
                    t.draw(vertices, 4, Lines);

                    NorthWest->draw(t);
                    NorthEast->draw(t);
                    SouthWest->draw(t);
                    SouthEast->draw(t);
                }
            }
            size_t checkCollision()
            {
                size_t collisionCount = 0;
                if(divided)
                {
                    collisionCount += NorthWest->checkCollision();
                    collisionCount += NorthEast->checkCollision();
                    collisionCount += SouthWest->checkCollision();
                    collisionCount += SouthEast->checkCollision();

                }
                else
                {
                    //check if 2 entities collide
                    for(vector<Entity*>::iterator i = children.begin(); i != children.end(); ++i)
                    {
                        for(vector<Entity*>::iterator j = i; j != children.end(); ++j)
                        {
                            if(i != j && (*i)->intersects(**j))
                            {
                                (*i)->collides = true;
                                (*j)->collides = true;
                            }
                            ++collisionCount;
                        }
                    }
                }
                return collisionCount;
            }

};
int main()
{
    srand(time(0));
    RenderWindow window(sf::VideoMode(SCREEN_W + 200, SCREEN_H), "Collision Plot Using Quad Trees");
    QuadTree *tree;
    Entity *entity;
    vector<Entity*> entities;
    vector<Entity*> found;
    RectangleShape shape;
    shape.setOutlineColor(Color::Blue);
    int width = 5, height = 5;
    float timeTree, timeBrute;
    int countTree, countBrute;
    Clock timer; //measures elapsed time
    Font font; //loading and manipulating character fonts
    font.loadFromFile("AlexBrush-Regular.ttf"); //downloaded AxelBrush
    Text text;//for text - we need font
    text.setFont(font);
    text.setCharacterSize(20);
    text.setColor(Color::Yellow);

    while(window.isOpen())
    {
        Event e;
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
            {
                window.close();
            }
            else if(e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                for(int i = 0; i < 10; ++i)
                {
                    entity = new Entity(Mouse::getPosition(window).x, Mouse::getPosition(window).y, width, height);
                    entity->angle = rand() % 360; //angle
                    entities.push_back(entity);
                }
            }
            else if (e.type == Event::MouseWheelScrolled)
            {
                if(Keyboard::isKeyPressed(Keyboard::LShift))
                {
                    width += e.mouseWheelScroll.delta * 5;
                }
                else
                {
                    height += e.mouseWheelScroll.delta * 5;
                }
            }
        }
        tree = new QuadTree(Rectangle(SCREEN_W / 2, SCREEN_H / 2, SCREEN_W / 2, SCREEN_H / 2), CAPACITY, 0);
        timeTree = 0;
        for(vector<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
        {
            (*it)->Move();
            (*it)->collides = false;
            timer.restart();
            tree->insert(*it);
            timeTree += timer.restart().asMicroseconds();
        }
        countTree = tree->checkCollision();
        timeTree += timer.restart().asMicroseconds();

        countBrute = 0;
        timer.restart();
        for(vector<Entity*>::iterator i = entities.begin(); i != entities.end(); ++i)
        {
            for(vector<Entity*>::iterator j = i; j != entities.end(); ++j)
            {
                if(i != j && (*i)->intersects(**j))
                {
                    (*j)->collides = true;
                    (*j)->collides = true;
                }
                ++countBrute;
            }
        }
        timeBrute = timer.restart().asMicroseconds();

        window.clear();
        shape.setOutlineThickness(-1);
        for(Entity *e : entities)
        {
            shape.setPosition(e->x, e->y);
            shape.setSize(Vector2f(e->w * 2, e->h * 2));
            shape.setOrigin(e->w, e->h);
            shape.setFillColor(e->collides?Color::Red:Color::Green);
            window.draw(shape);
        }
        tree->draw(window);
        shape.setOutlineThickness(0);
        shape.setPosition(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
        shape.setSize(Vector2f(width * 2, height * 2));
        shape.setOrigin(width, height);
        shape.setFillColor(Color(255, 255, 0, 50));
        window.draw(shape);

        text.setString("Tree count: " + intToString(countTree) +
                       "\nBrute force count: " + intToString(countBrute) +
                       "\nTree time: " + intToString(timeTree) +
                       "\nBrute force time:" + intToString(timeBrute) +
                       "\nTree is " +
                       intToString(round(timeBrute / timeTree)) + "times faster");
        text.setPosition(SCREEN_W + 10, 20);
        window.draw(text);

        window.display();
    }
    for(int i = 0; i < entities.size(); ++i)
    {
        delete entities[i];
    }
    delete tree;
    return 0;
}

