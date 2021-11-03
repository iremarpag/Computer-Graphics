#include "shape.h"
#include "consts.h"

void Shape::create() {}

void Shape::change_color(color4 color) {
    this->color = color;
    fill(colors.begin(), colors.end(), this->color);
}

void Shape::change_color_rgb() {
    for(int i = 0; i < this->numOfVertices; i++) {
        colors[i] = vertex_colors[i % 8]; // in consts.h
    }
}