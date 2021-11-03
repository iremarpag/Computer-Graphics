#include "cube.h"

void Cube::create() {
    points.resize(CUBE_VERTEX_COUNT);
    colors.resize(CUBE_VERTEX_COUNT);
    RGB.resize(CUBE_VERTEX_COUNT);
    numOfVertices = CUBE_VERTEX_COUNT;

    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);
}

void Cube::change_color_rgb() {
    Index = 0;
    create();
}

void Cube::quad(int a, int b, int c, int d) {
    colors[Index] = vertex_colors[a];
    points[Index] = vertices[a];
    Index++;
    colors[Index] = vertex_colors[b];
    points[Index] = vertices[b];
    Index++;
    colors[Index] = vertex_colors[c];
    points[Index] = vertices[c];
    Index++;
    colors[Index] = vertex_colors[a];
    points[Index] = vertices[a];
    Index++;
    colors[Index] = vertex_colors[d];
    points[Index] = vertices[d];
    Index++;
    colors[Index] = vertex_colors[c];
    points[Index] = vertices[c];
    Index++;
}
