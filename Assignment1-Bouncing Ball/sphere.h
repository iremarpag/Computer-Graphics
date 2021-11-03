#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"


const int SUBDIVISION_COUNT = 4;
const int VERTEX_COUNT =  pow(pow(2, SUBDIVISION_COUNT + 1), 2) * 3;


class Sphere : public Shape {

public:
  
  //Initial tetrahedron's vertices
  point4 v[4]= {
    vec4(0.0, 0.0, 1.0, 1.0),
    vec4(0.0, 0.942809, -0.333333, 1.0),
    vec4(-0.816497, -0.471405, -0.333333, 1.0),
    vec4(0.816497, -0.471405, -0.333333, 1.0)
  };
  int index = 0;
  
  void create();
  
  point4 unit(const point4&);
  void triangle(point4, point4, point4);
  void divide_triangle(point4, point4, point4, int);

};

#endif