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

#include "util.h"
#include "Config.h"
#include "GameManager.h"

#ifndef M_PI
#define M_PI 3.14159f
#endif

float aspect_ratio = 1.0f;
SceneNode * root_node = NULL; // scene root
TransformNode * ctrans = NULL; // we control cessna
SceneParams scene_params;

// --------------------------------------------------

//Called whenever the window is resized. The new window size is given, in pixels.
//This is an opportunity to call glViewport or glScissor to keep up with the change in size.

/*
 * Entry point
 */
int main(int argc, char** argv)
{
    // Not used at the moment
    //glutIdleFunc(&update_function);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    Config * c = Config::getInstance();
    c->parseFile("swoopers.cfg");

    int w = c->getInt("window_width");
    int h = c->getInt("window_height");
    // initial window size
    glutInitWindowSize(w, h);
    glutCreateWindow("Swoopers");

    // Enable blending
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glutDisplayFunc(GameManager::OnDisplay);
    // register callback for change of window
    glutReshapeFunc(GameManager::OnReshape);
    // register callback for keyboard
    glutTimerFunc(33, GameManager::OnTimer, 0);

    // load the pointers to OpenGL functions (only needed in MS Windows)
    glewInit();
    if(!GLEW_VERSION_3_0)
    {
	fprintf(stderr, "OpenGL 3.0 not available\n");
	return 1;
    }
    GameManager::Initialize();

    glutMainLoop();
    return 0;
}
