#include"Point.h"
using namespace std;
Point::Point()
{
    x = 0;
    y = 0;
    lights_up = false;
}
Point::Point(float x, float y, bool lights_up)
{
    this->x = x;
    this->y = y;
    this->lights_up = lights_up;
}

