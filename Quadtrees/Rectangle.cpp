#include<iostream>
#include"Rectangle.h"
Rectangle::Rectangle(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}
bool Rectangle::is_contained(const Point &P) const
{
    if(P.x >= x - w && P.x <= x + w && P.y >= y - h && P.y <= y + h)
    {
        return true;
    }
    return false;
}
bool Rectangle::region_intersect(const Rectangle &other) const
{
    return !(x - w > other.x + other.w || x + w < other.x - other.w || y - h > other.y + other.h || y + h < other.y - other.h);
}
/*
    if the point is outside the boundary - evaluates true - return false
*/
void Rectangle::Draw(RenderTarget &T)
{
    static Vertex vertices[5]; //a point with color and texture co-ordinates - SFML
    vertices[0] = Vertex(Vector2f(x - w, y - h), Color::Magenta);
    vertices[1] = Vertex(Vector2f(x + w, y - h), Color::Magenta);
    vertices[2] = Vertex(Vector2f(x + w, y + h), Color::Magenta);
    vertices[3] = Vertex(Vector2f(x - w, y + h), Color::Magenta);
    vertices[4] = Vertex(Vector2f(x - w, y - h), Color::Magenta);
    //Vertex constructor - Position, Color, Texture - (0, 0)
    //Texture - how should the point get mapped to the whole geometry - for common appearance
    //Vector2 - just the points
    T.draw(vertices, 5, LineStrip);
    //Linestrip - to draw connected lines - enum
}

