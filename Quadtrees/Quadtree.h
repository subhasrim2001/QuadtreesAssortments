#ifndef QUADTREE_H_INCLUDED
#define QUADTREE_H_INCLUDED
#include"Rectangle.h"
#include<iostream>
#include<vector>
class QuadTree {
    private:
            QuadTree *NorthWest;
            QuadTree *NorthEast;
            QuadTree *SouthWest;
            QuadTree *SouthEast;
            Rectangle boundary;
            bool is_divided;
            size_t capacity; //typedef unsigned int size_t
            size_t level; //level of division
            std::vector <Point*> children;
            void further_divide();
    public:
            QuadTree(const Rectangle &, size_t, size_t);
            ~QuadTree();
            bool insert(Point *);
            void query(const Rectangle &) const;
            void Draw(RenderTarget &);
            void dothis()
            {
                std::cout<<is_divided;
                if(is_divided)
                {
                    std::cout<<NorthWest->is_divided;
                }
            }

};

#endif // QUADTREE_H_INCLUDED
