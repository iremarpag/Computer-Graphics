#ifndef BUNNY_H
#define BUNNY_H
#include "shape.h"
#include <fstream>

typedef vec4  color4;
typedef vec4  point4;

class Bunny : public Shape {
public:
  std::vector<point4> vertices;
  std::vector<int> triangles;
  int numOfTriangles;

  void create();

private:
  void fill();

};

#endif