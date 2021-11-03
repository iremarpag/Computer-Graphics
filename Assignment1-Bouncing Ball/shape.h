#ifndef SHAPE_H
#define SHAPE_H
#include "include/Angel.h"
#include <vector>

using color4 = vec4;
using point4 = vec4;

class Shape {
public:
    int numOfVertices;
    std::vector<point4> points;
    std::vector<color4> colors;
    std::vector<color4> RGB;
    color4 color = color4(1.0, 0.0, 0.0, 1.0);

    virtual void create();
    virtual void change_color(color4);
    virtual void change_color_rgb();
};
#endif