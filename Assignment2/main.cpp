
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "include/Angel.h"
#include "rubiks_cube.h"


rubiks_cube cube;

float defX = 45.0; //Default X-axis rotation
float defY = 30.0; //Default Y-axis rotation
float rotationX = 45.0;
float rotationY = 30.0;
bool rotation_active = false;
GLfloat rotation_angle;
int rotation_face = 1; //Red, Yellow, Orange, Green, White, Blue

//Shuffle variables
bool shuffle_active;
int shuffle_dir;
int shuffle_count = 12; //Number of times a face will be rotated


//Dragging
GLfloat mouseX, mouseY;
GLfloat newX, newY;
bool mouseState = false;

// Model-view and projection matrices and color uniform location
GLuint  ModelView, Projection;
GLuint Color;

//  initialization
void init() {
  cube.create_rubiks_cube();

  point4 *rot_data, *color_data;

  rot_data = cube.get_rot_data();
  color_data = cube.get_color_data();

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers( 1, &buffer );
  glBindBuffer( GL_ARRAY_BUFFER, buffer );
  glBufferData( GL_ARRAY_BUFFER, (sizeof(point4) + sizeof(color4)) * FACE * VERTICES_NUM, NULL, GL_STATIC_DRAW);
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * FACE * VERTICES_NUM, rot_data);
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * FACE * VERTICES_NUM,
                     sizeof(color4) * FACE * VERTICES_NUM, color_data);

        
  // Load shaders and use the resulting shader program
  GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    
  // set up vertex arrays
  GLuint vPosition = glGetAttribLocation( program, "vPosition" );
  glEnableVertexAttribArray( vPosition );
  glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  
  GLuint vColor = glGetAttribLocation( program, "vColor" );
  glEnableVertexAttribArray( vColor );
  glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                         BUFFER_OFFSET(sizeof(points)) );


  // Retrieve transformation uniform variable locations
  ModelView = glGetUniformLocation( program, "ModelView" );
  Projection = glGetUniformLocation( program, "Projection" );

  // Set current program object
  glUseProgram( program );

  // Set projection matrix
  mat4  projection;
  projection = Ortho(-2.0, 2.0, -2.0, 2.0, -2.0, 2.0);
  glUniformMatrix4fv( Projection, 1, GL_TRUE, projection );

  mat4  model_view = (RotateX (rotationX ) *
                      RotateY( rotationY ));

  glUniformMatrix4fv( ModelView, 1, GL_TRUE, model_view );

  Color = glGetUniformLocation( program, "color" );

  // Enable hiddden surface removal
  glEnable( GL_DEPTH_TEST );

  // Set state variable "clear color" to clear buffer with.
  glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void display(  ) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUniform4f( Color, 1.0, 0.0, 0.0, 1.0 );

  color4* temp2 = cube.get_color_data();

  glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * FACE * VERTICES_NUM,
                     sizeof(color4) * FACE * VERTICES_NUM, temp2);

  if(rotation_active){
    cube.modify_rotation_array(rotation_face, rotation_angle);
  }

  mat4 model_view = (RotateX (rotationX ) *
                     RotateY( rotationY ));

  glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);
  glUniform4f( Color, 1.0, 0.0, 0.0, 1.0 );
  glDrawArrays( GL_TRIANGLES, 0, 36 * 4 * 8);

  glutSwapBuffers();
}


void mouse( int button, int state, int x, int y ) {
  if(shuffle_active)
    return;

  //Set the initial mouse reference points for rotating the cube
  if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
    mouseX = x;
    mouseY = y;
    mouseState = true;
  }

  //End cube rotation
  if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON) {
    mouseState = false;
    defX = rotationX;
    defY = rotationY;

    mouseX = x;
    mouseY = y;
  }

  //Set the initial mouse reference points for rotating a side and set the picking colors
  if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
    mouseX = x;
    mouseY = y;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    color4* new_colors = cube.set_picking_colors();
    point4* base_data = cube.get_base_data();


    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4) * FACE * VERTICES_NUM, base_data);
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4) * FACE * VERTICES_NUM, sizeof(color4) * FACE * VERTICES_NUM, new_colors);

    mat4 model_view = (RotateX (rotationX) *
                       RotateY (rotationY) );

    glUniformMatrix4fv(ModelView, 1, GL_TRUE, model_view);

    glDrawArrays( GL_TRIANGLES, 0, 36 * 4 * 8);

    glFlush();
    delete[] new_colors;

    y = glutGet( GLUT_WINDOW_HEIGHT ) - y;

    int side;
    unsigned char pixel[4];
    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    if (pixel[0]==25 && pixel[1]==25 && pixel[2]==25) side = 1;
    else if (pixel[0]==51 && pixel[1]==51 && pixel[2]==51) side = 2;
    else if(pixel[0]==76 && pixel[1]==76 && pixel[2]==76) side = 3;
    else if (pixel[0]==102 && pixel[1]==102 && pixel[2]==102) side = 4;
    else if (pixel[0]==128 && pixel[1]==128 && pixel[2]==128) side = 5;
    else if(pixel[0]==153 && pixel[1]==153 && pixel[2]==153) side = 6;
    else if(pixel[0]==0 && pixel[1]==0 && pixel[2]==0) side = 7; //Black edges
    else if(pixel[0]==255 && pixel[1]==255 && pixel[2]==255) side = 8; //Empty canvas
    else return;

    rotation_face = side;

    rotation_active = true;
  }

  //End rotating a side and approximate the rotation angle
  if ( state == GLUT_UP && button == GLUT_LEFT_BUTTON) {

    rotation_active = false;

    //approx: Side rotation approximation
    //approx2: Rotation count
    float approx, approx2;
    if(rotation_angle > -45 && rotation_angle < 45) {
      approx = 0;
      approx2 = 0;
    } else if (rotation_angle > 45 && rotation_angle < 135 ) {
      approx = 90;
      approx2 = 1;
    } else if (rotation_angle > 135 || rotation_angle < -135 ) {
      approx = 180;
      approx2 = 2;
    } else if (rotation_angle > -135 && rotation_angle < -45 ) {
      approx = 270;
      approx2 = 3;
    } else {
      return;
    }

    rotation_angle = 0;

    cube.modify_rotation_array(rotation_face, approx);
    cube.modify_face_array(rotation_face, approx2);


    glutPostRedisplay();
  }
}

void mouseMotion(int x, int y) {
  if(shuffle_active)
    return;

  newX = x;
  newY = y;

  //Calculate cube rotation
  if(mouseState) {
    rotationY = defY + newX - mouseX;
    rotationX = defX + newY - mouseY;
  }

  //Calculate face rotation angle
  if(rotation_active) {
    int vector1x = mouseX - 256;
    int vector1y = mouseY - 256;

    int vector2x = newX - 256;
    int vector2y = newY - 256;

    float dotProduct = vector1x * vector2x + vector1y * vector2y;
    float determinant = vector1x * vector2y - vector1y * vector2x;
    rotation_angle = std::atan2(determinant, dotProduct) * 180 / M_PI;
  }
}

void shuffle_cube() {
  srand(time(NULL));
  int rand1 = rand() % 6 + 1; //For random face
  rotation_face = rand1;
  int rand2 = rand() % 2; //For random direction
  shuffle_dir = rand2;
  rotation_active = true;
  shuffle_active = true;
}

void keyboard(unsigned char key, int x, int y) {

  switch(key) {
  case 's':
  case 'S':
    if(shuffle_active)
      return;
    shuffle_cube();
    break;
  case 'q':
  case 'Q':
    exit( EXIT_SUCCESS );
    break;
  case 'h':
  case 'H':
    std::cout << "\nHelp" << std::endl;
    std::cout << "-----" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "Shuffling" << std::endl;
    std::cout << "----------" << std::endl;
    std::cout << "-The cube shuffles itself randomly at the start of the program" << std::endl;
    std::cout << "-The cube can be shuffled manually by pressing 's'" << std::endl;
    std::cout << "-The cube cannot be rotated or modified during scrambling process" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "Rotating a face" << std::endl;
    std::cout << "---------------" << std::endl;
    std::cout << "-The cube is modified by pressing and holding the left mouse button on the side that is desired to be rotated" << std::endl;
    std::cout << "-For a more accurate rotation, the side that will be rotated should be faced by a user" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "Rotating the cube" << std::endl;
    std::cout << "------------------" << std::endl;
    std::cout << "-The cube can be rotated by pressing and holding the right mouse button and dragging the cube" << std::endl;
    std::cout << "" << std::endl;

    std::cout << "Program is terminated by pressing 'q'" << std::endl;
    break;
  }
}

void idle( void )
{
  //Shuffle operation
  if(shuffle_active) {
    if(rotation_angle >= 90 || rotation_angle <= -90) {
      rotation_active = false;
      shuffle_active = false;
      int rotation_count;
      if(shuffle_dir == 0) //clockwise 90 degree turn 
        rotation_count = 1;
      else                 //counter-clockwise 90 degree turn (three clockwise 90 degree turns)
        rotation_count = 3;
      cube.modify_face_array(rotation_face, rotation_count);
      rotation_angle = 0;
      if(shuffle_count > 1) {
        srand(time(NULL));
        int rand1 = (rand() + 10)% 6 + 1; //For random face
        rotation_face = rand1;
        int rand2 = rand() % 2; //For random direction
        //Set shuffle inputs
        shuffle_dir = rand2;
        shuffle_count--;
        rotation_active = true;
        shuffle_active = true;
      } else {
        shuffle_count = 12;
      }
    } else {
      if(shuffle_dir == 0)
        rotation_angle += 5.0;
      else
        rotation_angle -= 5.0;
    }
  }
  glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitContextVersion(3,2);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  glutInitWindowSize( 512, 512 );
  glutCreateWindow( "Mini Rubik's cube" );

  glewExperimental = GL_TRUE;
  glewInit();

  init();
  shuffle_cube();

  glutDisplayFunc( display ); // set display callback function
  glutIdleFunc( idle );
  glutKeyboardFunc( keyboard );
  glutMouseFunc( mouse );
  glutMotionFunc( mouseMotion );

  glutMainLoop();
  return 0;
}
