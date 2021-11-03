
#include "include/Angel.h"

typedef vec4 point4;
typedef vec4 color4;

#define FACE 8
#define VERTICES_NUM (36 * 4)

point4 points[FACE * VERTICES_NUM];

//An array that stores which square is on which face
int faces[6][4] = {
  {0, 1, 3, 2},
  {1, 5, 7, 3},
  {5, 4, 6, 7},
  {4, 0, 2, 6},
  {4, 5, 1, 0},
  {2, 3, 7, 6}
};

//An array that stores subcubes neighbouring to a face 
int cube_neighbours[6][4] = {
  {4, 1, 5, 3},
  {4, 2, 5, 0},
  {4, 3, 5, 1},
  {4, 0, 5, 2},
  {2, 1, 0, 3},
  {0, 1, 2, 3}
};

class rubiks_cube {

  point4 rot_arr[FACE][VERTICES_NUM]; //Rotation operation array
  point4 subcubes[FACE][VERTICES_NUM]; //Whole cube vertex array
  point4 base[FACE][VERTICES_NUM]; //Base array for coloring the picking colors 
  color4 colors[FACE][VERTICES_NUM]; //Color array

  // Vertices of two triangles
  point4 cube_vertices[FACE] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
  };

  color4 cube_colors[FACE] = {
    color4(0.0, 0.0, 0.0, 1.0), //Black
    color4(1.0, 1.0, 0.0, 1.0), //Yellow
    color4(1.0, 0.0, 0.0, 1.0), //Red
    color4(0.0, 0.0, 1.0, 1.0), //Blue
    color4(0.0, 1.0, 0.0, 1.0), //Green
    color4(1.0, 0.584, 0.0, 1.0), //Orange
    color4(1.0, 1.0, 1.0, 1.0) //White
  };
  int index = 0;

  public:

  void quad( int a, int b, int c, int d ) {
    points[index++] = cube_vertices[a]; 
    points[index++] = cube_vertices[b]; 
    points[index++] = cube_vertices[c]; 
    points[index++] = cube_vertices[a]; 
    points[index++] = cube_vertices[c]; 
    points[index++] = cube_vertices[d]; 
  }

  void generate() {
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
  }

  int mod(int a, int b) {
    return ((a % b) + b) % b;
  }

  void create_rubiks_cube() {

    generate();

    vec4 translations[] = {
      vec4(-0.5, 0.5, 0.5, 0.0),
      vec4(0.5, 0.5, 0.5, 0.0),
      vec4(-0.5, -0.5, 0.5, 0.0),
      vec4(0.5, -0.5, 0.5, 0.0),
      vec4(-0.5, 0.5, -0.5, 0.0),
      vec4(0.5, 0.5, -0.5, 0.0),
      vec4(-0.5, -0.5, -0.5, 0.0),
      vec4(0.5, -0.5, -0.5, 0.0)
    };
    
    int indices[][3] = {
      {1,2,6}, {1,5,6}, {1,2,3}, {1,5,3},
      {4,2,6}, {4,5,6}, {4,2,3}, {4,5,3}
    };

    vec4 shift[] = {
      vec4(0.0, 0.0, 0.00, 0.0),
      vec4(0.0, 0.0, 0.08, 0.0), // 1
      vec4(-0.08, 0.0, 0.0, 0.0), // 2
      vec4(0.0, -0.08, 0.0, 0.0), // 3
      vec4(0.0, 0.0, -0.08, 0.0), // 4
      vec4(0.08, 0.0, 0.0, 0.0), // 5
      vec4(0.0, 0.08, 0.0, 0.0), // 6
    };

    for(int i = 0; i < 36; i++) {

      point4 temp = points[i];
      vec4 translation;
      int index;
      
      for (int j=0; j<8; j++) {
        index = 0;
        translation = translations[j];
        subcubes[j][index + i] = temp + translation;
        colors[j][index + i] = cube_colors[0];
        index += 36;
        
        for(int k=0; k<3; k++) {
          subcubes[j][index + i] = Scale(0.90) * temp + shift[indices[j][k]] + translation;
          colors[j][index + i] = cube_colors[indices[j][k]];
          index += 36;
        }
      }
    }
    for(int i = 0; i < FACE; i++) {
      for(int j = 0; j < VERTICES_NUM; j++) {
        rot_arr[i][j] = subcubes[i][j];
        base[i][j] = subcubes[i][j];
      }
    }
  }
  
  point4* get_base_data() { return &base[0][0]; }
  point4* get_subcube_data() { return &subcubes[0][0]; }
  point4* get_rot_data() { return &rot_arr[0][0]; }
  color4* get_color_data() { return &colors[0][0]; }

  //Returns rotation angle and direction according to the face index
  mat4 getRotation(int face, float angle) {
    mat4 result;
    switch(face) {
    case 0:
      result = RotateZ(-angle);
      break;
    case 1:
      result = RotateX(-angle);
      break;
    case 2:
      result = RotateZ(angle);
      break;
    case 3:
      result = RotateX(angle);
      break;
    case 4:
      result = RotateY(-angle);
      break;
    case 5:
      result = RotateY(angle);
      break;
    }
    return result;
  }

  //Modifies the vertex array when a side rotation occurs
  void modify_rotation_array(int rotation_face, float angle) {
    int face = rotation_face - 1;

    //White spaces and black parts of the cube
    if(face == 6 || face == 7) {
      glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(rot_arr), rot_arr);
      return;
    }

    for(int i = 0; i < 4; i++){
      for(int j = 0; j < VERTICES_NUM; j++) {
        int index = faces[face][i];
        rot_arr[index][j] = getRotation(face, angle) * subcubes[index][j];
      }
    }
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(rot_arr), rot_arr);
  }

  //Modifies the array that holds which face is in which side according to the rotation 
  void modify_face_array_helper(int face) {

    int temp_arr[6][4];
    std::copy(&faces[0][0], &faces[0][0]+6*4, &temp_arr[0][0]);

    //Rotated face
    for(int i = 0; i < 4; i++) {
      faces[face][(i+1) % 4] = temp_arr[face][i];
    }

    if(face < 4) {
      //Top-to-Right
      faces[cube_neighbours[face][1]][3] = temp_arr[cube_neighbours[face][0]][mod(3-face-1, 4)];
      faces[cube_neighbours[face][1]][0] = temp_arr[cube_neighbours[face][0]][3-face];

      //Right-to-Bottom
      faces[cube_neighbours[face][2]][face] = temp_arr[cube_neighbours[face][1]][3];
      faces[cube_neighbours[face][2]][mod(face+1, 4)] = temp_arr[cube_neighbours[face][1]][0];

      //Bottom-to-Left
      faces[cube_neighbours[face][3]][1] = temp_arr[cube_neighbours[face][2]][face];
      faces[cube_neighbours[face][3]][2] = temp_arr[cube_neighbours[face][2]][mod(face+1, 4)];

      //Left-to-Top
      faces[cube_neighbours[face][0]][mod(3-face-1, 4)] = temp_arr[cube_neighbours[face][3]][1];
      faces[cube_neighbours[face][0]][3-face] = temp_arr[cube_neighbours[face][3]][2];

    } else if(face == 4 ) {
      //All same indices for top face
      for(int i = 0; i < 4; i++){
        faces[cube_neighbours[face][i]][0] = temp_arr[cube_neighbours[face][mod(i-1, 4)]][0];
        faces[cube_neighbours[face][i]][1] = temp_arr[cube_neighbours[face][mod(i-1, 4)]][1];
      } 
    } else if(face == 5 ) {
      //All same indices for bottom face
      for(int i = 0; i < 4; i++){
        faces[cube_neighbours[face][i]][2] = temp_arr[cube_neighbours[face][mod(i-1, 4)]][2];
        faces[cube_neighbours[face][i]][3] = temp_arr[cube_neighbours[face][mod(i-1, 4)]][3];
      }
    }
  }

  void modify_face_array(int rotation_face, int repeat) {

    int face = rotation_face - 1;

    if(face == 6 || face == 7) {
      return;
    }

    for(int i = 0; i < repeat; i++) {
      modify_face_array_helper(face); 
    }
    for(int i = 0; i < FACE; i++) {
      for(int j = 0; j < VERTICES_NUM; j++) {
        subcubes[i][j] = rot_arr[i][j];
      }
    }
    
    
  }

  //Sets picking colors
  color4* set_picking_colors() {
    color4* new_colors = new color4[FACE * VERTICES_NUM];

    int vals[][3] = { {0,4,1}, {1,FACE,2}, {4,FACE,1}, {0,7,2},
      {0,2,1}, {4,6,1}, {2,4,1}, {6,8,1} 
    };

    int inner_vals[][2] = { {1,2}, {2,3}, {1,2}, {2,3},
      {3,4}, {3,4}, {3,4}, {3,4}
    };
    
    color4 colors[] = {
      color4(0.1, 0.1, 0.1, 1.0), // FRONT
      color4(0.2, 0.2, 0.2, 1.0), // RIGHT
      color4(0.3, 0.3, 0.3, 1.0), // BACK
      color4(0.4, 0.4, 0.4, 1.0), // LEFT
      color4(0.5, 0.5, 0.5, 1.0), // TOP
      color4(0.5, 0.5, 0.5, 1.0), // TOP
      color4(0.6, 0.6, 0.6, 1.0), // BOTTOM
      color4(0.6, 0.6, 0.6, 1.0), // BOTTOM
    };
    
    for(int k=0;k<8;k++) {
      for(int i = vals[k][0]; i < vals[k][1]; i+=vals[k][2]) 
        for(int j = 36 * inner_vals[k][0]; j < 36 * inner_vals[k][1]; j++) 
          new_colors[(i * VERTICES_NUM) + j] = colors[k];
        
    }
    
    return new_colors;
  }
};
