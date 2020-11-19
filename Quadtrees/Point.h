#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED
class Point
{
    public:
            float x;
            float y;
            bool lights_up;
            Point();
            Point(float, float, bool = false);
};

#endif // POINT_H_INCLUDED
