#ifndef CONSTS_H
#define CONSTS_H
#include "include/Angel.h"

using color4 = vec4;
using point4 = vec4;

// Vertices of a unit cube centered at origin, sides aligned with axes
const point4 vertices[8] = {
  point4( -1.0, -1.0,  1.0, 1.0 ),
  point4( -1.0,  1.0,  1.0, 1.0 ),
  point4(  1.0,  1.0,  1.0, 1.0 ),
  point4(  1.0, -1.0,  1.0, 1.0 ),
  point4( -1.0, -1.0, -1.0, 1.0 ),
  point4( -1.0,  1.0, -1.0, 1.0 ),
  point4(  1.0,  1.0, -1.0, 1.0 ),
  point4(  1.0, -1.0, -1.0, 1.0 )
};

// RGBA olors
const color4 vertex_colors[8] = {
  color4( 0.0, 0.0, 0.0, 1.0 ),  // black
  color4( 1.0, 0.0, 0.0, 1.0 ),  // red
  color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
  color4( 0.0, 1.0, 0.0, 1.0 ),  // green
  color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
  color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
  color4( 1.0, 1.0, 1.0, 1.0 ),  // white
  color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};


#endif