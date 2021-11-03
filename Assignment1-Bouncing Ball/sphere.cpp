#include "sphere.h"


//Creates a sphere recursively using tetrahedron-subdivison n times
void Sphere::create() {
    int n = SUBDIVISION_COUNT;
    points.resize(VERTEX_COUNT);
    colors.resize(VERTEX_COUNT);
    RGB.resize(VERTEX_COUNT);
    numOfVertices = VERTEX_COUNT;

    divide_triangle(v[0], v[1], v[2], n);
    divide_triangle(v[3], v[2], v[1], n );
    divide_triangle(v[0], v[3], v[1], n );
    divide_triangle(v[0], v[3], v[2], n );
}


//Get unit vector of a vector
point4 Sphere::unit(const point4 &p) {
    point4 c;
    double d = 0.0;
    for (int i = 0; i < 3; i++)
        d += p[i] * p[i];
    d = sqrt(d);
    if (d > 0.0)
        for (int i = 0; i < 3; i++)
            c[i] = p[i] / d;
    c[3] = 1.0;
    return c;
}

//Fills the point array with the input point objects
//and color array with the initial color
void Sphere::triangle(point4 a, point4 b, point4 c) {
    points[index] = a;
    colors[index] = color;
    index++;
    points[index] = b;
    colors[index] = color;
    index++;
    points[index] = c;
    colors[index] = color;
    index++;
}

//Performs subdivision on a triangle recursively until n is 0
void Sphere::divide_triangle(point4 a, point4 b, point4 c, int n) {
    point4 v1, v2, v3;
    if (n > 0)
    {
        v1 = unit(a + b);
        v2 = unit(a + c);
        v3 = unit(b + c);
        divide_triangle(a, v2, v1, n - 1);
        divide_triangle(c, v3, v2, n - 1);
        divide_triangle(b, v1, v3, n - 1);
        divide_triangle(v1, v2, v3, n - 1);
    }
    else
        triangle(a, b, c);
}