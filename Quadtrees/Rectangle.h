#ifndef RECTANGLE_H_INCLUDED
#define RECTANGLE_H_INCLUDED
#include <SFML/Graphics.hpp>
#include"Point.h"
using namespace sf;
class Rectangle
{
    public:
            float x;
            float y;
            float w;
            float h;
            Rectangle(float, float, float, float);
            bool is_contained(const Point &) const;
            bool region_intersect(const Rectangle &) const;
            void Draw(RenderTarget &);
};
#endif // RECTANGLE_H_INCLUDED

/*
        ____________
        |           |   |
        |           |   |h
        |     O     |   |
        |           |
        |___________|
        _______
            w
        center point - (x, y)
*/
