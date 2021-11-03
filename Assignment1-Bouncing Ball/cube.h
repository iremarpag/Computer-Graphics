#ifndef CUBE_H
#define CUBE_H
#include "shape.h"
#include "consts.h"


const int CUBE_VERTEX_COUNT = 36;

class Cube : public Shape {
public:
  
  int Index = 0;
  void create();
  void change_color_rgb();
  void quad(int,int,int,int);
};

#endif