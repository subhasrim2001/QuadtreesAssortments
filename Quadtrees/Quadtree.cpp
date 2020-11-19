#include"Quadtree.h"
#include<iostream>
#define MAXIMUM_LEVEL 6
QuadTree::QuadTree(const Rectangle &_boundary, size_t capacity, size_t level): boundary(_boundary)
{
    NorthWest = NULL;
    NorthEast = NULL;
    SouthWest = NULL;
    SouthEast = NULL;
    this->capacity = capacity;
    is_divided = false;
    if(level >= MAXIMUM_LEVEL) //level max - 6 - can change if needed
    {
        this->capacity = 0;
    }
}
QuadTree::~QuadTree()
{
    if(is_divided) //if divided - no longer need to store
    {
        delete NorthWest;
        delete NorthEast;
        delete SouthWest;
        delete SouthEast;
    }
}
void QuadTree::further_divide()
{
    static Vector2f halfsize; //vector
    halfsize.x = boundary.w / 2.0f;
    halfsize.y = boundary.h / 2.0f;
    NorthWest = new QuadTree(Rectangle (boundary.x - halfsize.x, boundary.y - halfsize.y, halfsize.x, halfsize.y), capacity, level + 1);
    NorthEast = new QuadTree(Rectangle (boundary.x + halfsize.x, boundary.y - halfsize.y, halfsize.x, halfsize.y), capacity, level + 1);
    SouthWest = new QuadTree(Rectangle (boundary.x - halfsize.x, boundary.y + halfsize.y, halfsize.x, halfsize.y), capacity, level + 1);
    SouthEast = new QuadTree(Rectangle (boundary.x + halfsize.x, boundary.y + halfsize.y, halfsize.x, halfsize.y), capacity, level + 1);
    is_divided = true;
}
bool QuadTree::insert(Point *P)
{
    //if boundary defined cannot store it - return false
    if(!boundary.is_contained(*P))
    {
        return false;
    }
    if(!is_divided)
    {
        //if we haven't divided it yet, insert in that boundary first
        children.push_back(P);
        //if capacity exceeds, subdivide and try to insert within one of the 4 subdivided parts
        if(children.size() > capacity && capacity!=0)
        {
            further_divide();//divide into 4 smaller rectangles
            std::vector<Point*>::iterator it = children.begin();
            while(it != children.end())
            {
                //add in new rectangle
                if(NorthWest->insert(*it));
                else if(NorthEast->insert(*it));
                else if(SouthWest->insert(*it));
                else if(SouthEast->insert(*it));

                //remove from old rectangle
                it = children.erase(it);
            }
        }
        return true;
    }
    else
    {
        //if it has been divided - try to insert further - recursive call
        if(NorthWest->insert(P))
        {
            return true;
        }
        else if(NorthEast->insert(P))
        {
            return true;
        }
        else if(SouthWest->insert(P))
        {
            return true;
        }
        else if(SouthEast->insert(P))
        {
            return true;
        }
        return false;
    }
}
//to check if a point has been found within a location
void QuadTree::query(const Rectangle &area) const
{
    if(!area.region_intersect(boundary))
    {
        return;
        //if the region doesnt intersect - leave
    }
    if(is_divided)
    {
        //if divided - check if subregions intersect
        NorthWest->query(area);
        NorthEast->query(area);
        SouthWest->query(area);
        SouthEast->query(area);
    }
    else
    {
        //if the region intersects and its not divided
        for(size_t i = 0; i < children.size(); ++i)
        {
            //if the area contains the child
            if(area.is_contained(*children[i]))
            {
                //that child - point has been found
                children[i]->lights_up = true;

            }
        }
    }
}
void QuadTree::Draw(RenderTarget &T)
{
    //to draw grids
    if(is_divided)
    {
        static Vertex vertices[4];//a point with color and texture co-ordinates - SFML
        vertices[0] = Vertex(Vector2f(boundary.x, boundary.y - boundary.h), Color::White);
        vertices[1] = Vertex(Vector2f(boundary.x, boundary.y + boundary.h), Color::White);
        vertices[2] = Vertex(Vector2f(boundary.x - boundary.w, boundary.y), Color::White);
        vertices[3] = Vertex(Vector2f(boundary.x + boundary.w, boundary.y), Color::White);
        T.draw(vertices, 4, Lines); //Lines - individual lines
        NorthWest->Draw(T);
        NorthEast->Draw(T);
        SouthWest->Draw(T);
        SouthEast->Draw(T);
    }
}
/*
        further_divide()
        ____________
        |           |   |
        |           |   |b.h
        |     O     |   |
        |           |
        |___________|
        _______
            b.w
        center point - (b.x, b.y)   b - boundary

        ____________
        |     |     |
        |  0  |  0  |
        |____ O ____|
        |  0  |  0  |
        |_____|_____|

        calculate the 4 - 0s (Note: y axis is backwards)

*/
