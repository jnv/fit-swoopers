// BI-PGR, 2011, put together by Vlastimil Havran and Tomas Barak
#include <string>
#include <vector>
#include <cstdio>
#include <string.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>

#include "global.h"
#include "Matrix4.h"
#include "util.h"
#include "PyramidNode.h"
#include "TransformNode.h"
#include "RotationAnimNode.h"
#include "TerrainNode.h"
#include "MeshNode.h"
#include "SceneParams.h"
#include "CameraManager.h"
#include "CameraNode.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

float aspect_ratio = 1.0f;
SceneNode * root_node = NULL; // scene root
TransformNode * pyra_trans = NULL; // we control cessna
SceneParams scene_params;

// -------------------------------------------------------------------
void FuncTimerCallback(int)
{
  double timed = 0.001 * (double)glutGet(GLUT_ELAPSED_TIME); // milliseconds

  if(root_node)
     root_node->update(timed);

  glutTimerFunc(33, FuncTimerCallback, 0);
  glutPostRedisplay();
}

void functionDraw()
{
  glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(root_node)
    root_node->draw(&scene_params);
}

// --------------------------------------------------
// General functions

void InitializeScene()
{
  scene_params.view_mat = Matrix4f::Identity();
  scene_params.projection_mat = Matrix4f::Perspective(M_PI / 4, aspect_ratio, 0.1f, 100);

  CameraManager::getInstance()->setSceneParams(&scene_params);

  // create scene root node
  root_node = new SceneNode("root");

  // create transformation for the terrain
  TransformNode * ptrans = new TransformNode("ter-trans3", root_node);
  ptrans->translate(Vec3f(0, 0, -9));  
  ptrans->rotate(0.25, Vec3f(1,0,0));
  ptrans->scale(Vec3f(12, 0.2, 12));
  TerrainNode *terrain = new TerrainNode("terrain1", ptrans);
  terrain->load("data/terrain");

  // create transform matrix for the car
  TransformNode * ctrans = new TransformNode("c-trans1", root_node);
  ctrans->translate(Vec3f(1.0, -0.53, -5.4));
  ctrans->rotate(0.18, Vec3f(1.0, 0.0, 0));
  ctrans->rotate(-1.1, Vec3f(0.0, 1.0, 0));
  ctrans->rotate(0.02, Vec3f(1.0, 0.0, 1.0));
  ctrans->scale(Vec3f(0.02, 0.02, 0.02));
  // load the car as the mesh
  MeshNode * car = new MeshNode("data/bronco.obj", ctrans);
  car->loadMesh();
  car->printBBoxSize();

#if 0
  // the replacement for the left reflector by pyramid
  pyra_trans = new TransformNode("pyr-trans2", root_node);
  pyra_trans->translate(Vec3f(0.309363, -0.74156, -4.86074));
  pyra_trans->rotate(-1.0, Vec3f(0, 1, 0));
  pyra_trans->scale(Vec3f(0.1, 0.1, 0.1));
  PyramidNode *pyra1 = new PyramidNode("pyra", pyra_trans);
#endif

#if 0
  // the replacement for the right reflector by pyramid
  TransformNode *pyra_trans2 = new TransformNode("pyr-trans2", root_node);
  pyra_trans2->translate(Vec3f(0.123297, -0.68428, -5.200));
  pyra_trans2->rotate(-1.0, Vec3f(0, 1, 0));
  pyra_trans2->scale(Vec3f(0.1, 0.1, 0.1));
  PyramidNode *pyra2 = new PyramidNode("pyra", pyra_trans2);
#endif

  // dump our scene graph tree for debug
  root_node->dump();
}

//Called to update the display.
//You should call glutSwapBuffers after all of your rendering to display what you rendered.
//If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
void display()
{
  functionDraw();
  glutSwapBuffers();
}

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.
void reshape (int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  aspect_ratio = (float)w/(float)h;
  scene_params.projection_mat = Matrix4f::Perspective(M_PI / 4, aspect_ratio, 0.1f, 100);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to
//exit the program.
void myKeyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case 27:
      exit(0);//glutLeaveMainLoop();
      break;
    // Operations that move pyramid around the scene
	// and enable to orient in the scene
    case 'a':
      pyra_trans->rotate(0.02, Vec3f(1, 0, 0));
      break;
    case 'd':
      pyra_trans->rotate(-0.02, Vec3f(1, 0, 0));
      break;
    case 's':
      pyra_trans->rotate(0.02, Vec3f(0, 1, 0));
      break;
    case 'w':
      pyra_trans->rotate(-0.02, Vec3f(0, 1, 0));
      break;
    case 'e':
      pyra_trans->rotate(0.02, Vec3f(0, 0, 1));
      break;
    case 'q':
      pyra_trans->rotate(-0.02, Vec3f(0, 0, 1));
      break;
    case 'y':
      pyra_trans->translate(Vec3f(0.1, 0, 0));
      break;
    case 'u':
      pyra_trans->translate(Vec3f(-0.1, 0, 0));
      break;
    case 'h':
      pyra_trans->translate(Vec3f(0.0, 0.1, 0));
      break;
    case 'j':
      pyra_trans->translate(Vec3f(0.0, -0.1, 0));
      break;
    case 'n':
      pyra_trans->translate(Vec3f(0.0, 0.0, 0.1));
      break;
    case 'm':
      pyra_trans->translate(Vec3f(0.0, 0, -0.1));
      break;
  }

  // print out the matrix
  pyra_trans->debug();

}

//Called after the window and OpenGL are initialized. Called exactly once, before the main loop.
void init()
{
  InitializeScene();
  glDisable(GL_CULL_FACE); // draw both back and front faces
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
}

void mySpecialKeyboard(int specKey, int x, int y)
{
  switch (specKey)
  {
    case GLUT_KEY_UP:
      scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0, 0.1)) * scene_params.view_mat;
      break;
    case GLUT_KEY_DOWN:
      scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0, -0.1)) * scene_params.view_mat;
      break;
    case GLUT_KEY_F11:
      scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0.1, 0)) * scene_params.view_mat;
      break;
    case GLUT_KEY_F12:
      scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, -0.1, 0)) * scene_params.view_mat;
      break;
    case GLUT_KEY_F9:
      scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0.1, 0, 0)) * scene_params.view_mat;
      break;
    case GLUT_KEY_F10:
      scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(-0.1, 0, 0)) * scene_params.view_mat;
      break;
    case GLUT_KEY_LEFT:
      scene_params.view_mat.rotate(-0.1, Vec3f(0, 1, 0));
      break;
    case GLUT_KEY_RIGHT:
      scene_params.view_mat.rotate(0.1, Vec3f(0, 1, 0));
      break;
    case GLUT_KEY_PAGE_UP:
      scene_params.view_mat.rotate(0.1, Vec3f(0, 0, 1));
      break;
    case GLUT_KEY_PAGE_DOWN:
      scene_params.view_mat.rotate(-0.1, Vec3f(0, 0, 1));
      break;
    case GLUT_KEY_HOME:
      scene_params.view_mat.rotate(0.1, Vec3f(1, 0, 0));
      break;
    case GLUT_KEY_END:
      scene_params.view_mat.rotate(-0.1, Vec3f(1, 0, 0));
      break;
  }
}

/*
 * Entry point
 */
int main(int argc, char** argv)
{
  // initialize windonwing system
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  // initial window size
  glutInitWindowSize(800, 600);
  glutCreateWindow("Swoopers");

  // Not used at the moment
  //glutIdleFunc(&update_function);

  glutDisplayFunc(display);
  // register callback for change of window
  glutReshapeFunc(reshape);
  // register callback for keyboard
  glutKeyboardFunc(myKeyboard);
  glutSpecialFunc(mySpecialKeyboard);
  glutTimerFunc(33, FuncTimerCallback, 0);

  // load the pointers to OpenGL functions (only needed in MS Windows)
  glewInit();
  if (!GLEW_VERSION_3_0) {
    fprintf(stderr, "OpenGL 3.0 not available\n");
    return 1;
  }
  init();

  glutMainLoop();
  return 0;
}
