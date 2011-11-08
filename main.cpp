// BI-PGR, 2011, put together by Vlastimil Havran and Tomas Barak
#include <string>
#include <vector>
#include <cstdio>
#include <string.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "global.h"
#include "util.h"
#include "PyramidNode.h"
#include "TransformNode.h"
#include "RotationAnimNode.h"
#include "TerrainNode.h"
#include "MeshNode.h"
#include "SceneParams.h"
#include "CameraManager.h"
#include "CameraNode.h"
#include "InputManager.h"
#include "SwoopManager.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

float aspect_ratio = 1.0f;
SceneNode * root_node = NULL; // scene root
TransformNode * ctrans = NULL; // we control cessna
SceneParams scene_params;

// -------------------------------------------------------------------

void FuncTimerCallback(int)
{
    double timed = 0.001 * (double) glutGet(GLUT_ELAPSED_TIME); // milliseconds

    if(root_node)
	root_node->update(timed);
    InputManager::getInstance()->update();

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

    scene_params.view_mat = glm::mat4();
    /*glm::lookAt(glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, -1, 0) // Head is up (set to 0,-1,0 to look upside-down)
		);*/
    scene_params.projection_mat = glm::perspective(
						   45.f, // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
						   aspect_ratio, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
						   0.1f, // Near clipping plane. Keep as big as possible, or you'll get precision issues.
						   100.0f // Far clipping plane. Keep as little as possible.
						   );

    CameraManager::getInstance()->setSceneParams(&scene_params);

    // create scene root node
    root_node = new SceneNode("root");
    CameraNode * cam_global = new CameraNode("cam_global", root_node);

    // create transformation for the terrain
    TransformNode * ptrans = new TransformNode("ter-trans3", root_node);
    ptrans->translate(glm::vec3(0, -0.5, -10));
//    ptrans->rotate(10.f, glm::vec3(1, 0, 0));
    ptrans->scale(glm::vec3(12, 0.2, 12));
    TerrainNode *terrain = new TerrainNode("terrain1", ptrans);
    terrain->load("data/terrain");

    TransformNode * strans = SwoopManager::Initialize();
    strans->setParentNode(root_node);

    //XXX Animated camera... Why the hell not?
    RotationAnimNode * cam_anim = new RotationAnimNode("cam_anim", root_node);
    //    cam_car_trans->translate(glm::vec3(1.0, -0.53, -5.4));
    //    cam_car_trans->scale(glm::vec3(0.5, 0.5, 0.5));
    cam_anim->setSpeed(.1);
    cam_anim->setAxis(glm::vec3(0, 1, 0));
//    cam_car_trans->rotate(90.f, glm::vec3(1, 0, 0));
    CameraNode * cam2 = new CameraNode("cam2", cam_anim);


#if 0
    // the replacement for the left reflector by pyramid
    pyra_trans = new TransformNode("pyr-trans2", root_node);
    pyra_trans->translate(glm::vec3(0.309363, -0.74156, -4.86074));
    pyra_trans->rotate(-1.0, glm::vec3(0, 1, 0));
    pyra_trans->scale(glm::vec3(0.1, 0.1, 0.1));
    PyramidNode *pyra1 = new PyramidNode("pyra", pyra_trans);
#endif

#if 0
    // the replacement for the right reflector by pyramid
    TransformNode *pyra_trans2 = new TransformNode("pyr-trans2", root_node);
    pyra_trans2->translate(glm::vec3(0.123297, -0.68428, -5.200));
    pyra_trans2->rotate(-1.0, glm::vec3(0, 1, 0));
    pyra_trans2->scale(glm::vec3(0.1, 0.1, 0.1));
    PyramidNode *pyra2 = new PyramidNode("pyra", pyra_trans2);
#endif

    // dump our scene graph tree for debug
    root_node->dump();

    InputManager::Initialize();
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

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    aspect_ratio = (float) w / (float) h;
    scene_params.projection_mat = glm::perspective(45.f, aspect_ratio, 0.1f, 100.0f);
}

//Called whenever a key on the keyboard was pressed.
//The key is given by the ''key'' parameter, which is in ASCII.
//It's often a good idea to have the escape key (ASCII value 27) call glutLeaveMainLoop() to
//exit the program.

void myKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
	exit(0); //glutLeaveMainLoop();
	break;
	// Operations that move pyramid around the scene
	// and enable to orient in the scene
    case 'a':
	ctrans->rotate(15, glm::vec3(1, 0, 0));
	break;
    case 'd':
	ctrans->rotate(-15, glm::vec3(1, 0, 0));
	break;
    case 's':
	ctrans->rotate(15, glm::vec3(0, 1, 0));
	break;
    case 'w':
	ctrans->rotate(-15, glm::vec3(0, 1, 0));
	break;
    case 'e':
	ctrans->rotate(15, glm::vec3(0, 0, 1));
	break;
    case 'q':
	ctrans->rotate(-15, glm::vec3(0, 0, 1));
	break;
    case 'y':
	ctrans->translate(glm::vec3(0.1, 0, 0));
	break;
    case 'u':
	ctrans->translate(glm::vec3(-0.1, 0, 0));
	break;
    case 'h':
	ctrans->translate(glm::vec3(0.0, 0.1, 0));
	break;
    case 'j':
	ctrans->translate(glm::vec3(0.0, -0.1, 0));
	break;
    case 'n':
	ctrans->translate(glm::vec3(0.0, 0.0, 0.1));
	break;
    case 'm':
	ctrans->translate(glm::vec3(0.0, 0, -0.1));
	break;
    case 'c':
	CameraManager::getInstance()->nextCamera();
    }

    // print out the matrix
    ctrans->debug();

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
    CameraManager * cman = CameraManager::getInstance();

    switch(specKey)
    {
    case GLUT_KEY_UP:
	cman->translate(glm::vec3(0, 0, -1));
	break;
    case GLUT_KEY_DOWN:
	cman->translate(glm::vec3(0, 0, 1));
	//	scene_params.view_mat = Matrix4<float>::FromTranslation(glm::vec3(0, 0, -0.1)) * scene_params.view_mat;
	break;
    case GLUT_KEY_F11:
	//	scene_params.view_mat = Matrix4<float>::FromTranslation(glm::vec3(0, 0.1, 0)) * scene_params.view_mat;
	break;
    case GLUT_KEY_F12:
	//	scene_params.view_mat = Matrix4<float>::FromTranslation(glm::vec3(0, -0.1, 0)) * scene_params.view_mat;
	break;
    case GLUT_KEY_F9:
	//	scene_params.view_mat = Matrix4<float>::FromTranslation(glm::vec3(0.1, 0, 0)) * scene_params.view_mat;
	break;
    case GLUT_KEY_F10:
	//	scene_params.view_mat = Matrix4<float>::FromTranslation(glm::vec3(-0.1, 0, 0)) * scene_params.view_mat;
	break;
    case GLUT_KEY_LEFT:
	cman->rotate(-15, glm::vec3(0, 1, 0));
	break;
    case GLUT_KEY_RIGHT:
	cman->rotate(15, glm::vec3(0, 1, 0));
	break;
    case GLUT_KEY_PAGE_UP:
	cman->translate(glm::vec3(0, 1, 0));
	break;
    case GLUT_KEY_PAGE_DOWN:
	cman->translate(glm::vec3(0, -1, 0));
	break;
    case GLUT_KEY_HOME:
	cman->rotate(15, glm::vec3(1, 0, 0));
	break;
    case GLUT_KEY_END:
	cman->rotate(-15, glm::vec3(1, 0, 0));
	break;
    }
    //    scene_params.view_mat.dump();
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
    glutTimerFunc(33, FuncTimerCallback, 0);

    // load the pointers to OpenGL functions (only needed in MS Windows)
    glewInit();
    if(!GLEW_VERSION_3_0)
    {
	fprintf(stderr, "OpenGL 3.0 not available\n");
	return 1;
    }
    init();

    glutMainLoop();
    return 0;
}
