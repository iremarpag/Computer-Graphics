#include "bunny.h"

//Creates the bunny object by reading the points and their configuation
//from the bunny.off file
void Bunny::create() {
    std::string input;
    int i;

    std::ifstream file("bunny.off"); //Read bunny.off
    file >> input;                   //"OFF" word
    file >> numOfVertices >> numOfTriangles >> i;

    //Put the vertices to the vertices vector one by one
    for (i = 0; i < numOfVertices; i++) {
        GLfloat x, y, z;
        file >> x >> y >> z;
        vertices.push_back(point4(x, y, z, (GLfloat)1.0));
    }

    //Put the vertex alignment numbers to the triangles vector one by one
    for (i = 0; i < numOfTriangles; i++) {
        int dummy;
        int x, y, z;
        file >> dummy >> x >> y >> z;

        triangles.push_back(x);
        triangles.push_back(y);
        triangles.push_back(z);
    }

    file.close();

    fill();
}

//Iterates over triangles vector and puts the vertices to the points vector
//according to their alignment numbers in the triangles vector
void Bunny::fill() {
    for (auto it = triangles.begin(); it != triangles.end(); ++it) {
        points.push_back(vertices.at(*it));
        colors.push_back(color);
    }
    numOfVertices = points.size();
}