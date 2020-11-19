#include<iostream>
#include <SFML/Graphics.hpp>
#include"Point.h"
#include"Quadtree.h"
using namespace std;
using namespace sf; //For SFML functions
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define MAXIMUM_CAPACITY 4
int main()
{
    RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Collision via Quad Trees");
    //renderwindow class for 2D drawing - (Videomode, Windowname)
    //VideoMode class for VideoMode(width, height, bitsperpixel)
    QuadTree my_screen(Rectangle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2), MAXIMUM_CAPACITY, 0);
    //Rectangle initially has screen size - x = 300, y = 300, half sizes = 300, capacity = 2, level = 0
    Point *PC; //point clicked
    vector<Point*> points; //A vector of points
    CircleShape shape; //represents a circle
    shape.setRadius(3); //radius of the circle
    shape.setOrigin(3, 3); //sets the origin location of the  circle

    Rectangle area(200, 200, 100, 100);

    while(window.isOpen())
    {
        Event E;//describes a system event - like mouse click
        while(window.pollEvent(E)) //get the recent event to process it, fails when no pending event - retrieve
        {
            if(E.type == Event::Closed) //type - public attribute of event - type enum, Closed - integral constant
            {
                window.close();
            }
            else if(E.type == Event::MouseButtonPressed && E.mouseButton.button == Mouse::Left)//union datatype
            {
                //if user clicks a point on screen - store it
                PC = new Point(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
                points.push_back(PC);
                my_screen.insert(PC);//insert into quad tree
                my_screen.dothis();
            }
            else if(e.type == Event::MouseMoved)
            {

            }
        }
        for(size_t i = 0; i < points.size(); ++i)
        {
            points[i]->lights_up = false; //all points are initially not highligted
        }
        my_screen.query(area); //get all points found in the region

        window.clear();
        //display points
        for(Point *P : points) //iterates over begin and end iterators on points vector - P = *it
        {
            shape.setPosition(P->x, P->y);//position of the obj
            shape.setFillColor(P->lights_up?Color::Green:Color::White);//fill color of the shape
            window.draw(shape);
        }
        my_screen.Draw(window);
        area.Draw(window);
        window.display();
    }

}
