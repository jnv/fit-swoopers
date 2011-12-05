/* 
 * File:   GameManager.cpp
 * Author: j
 * 
 * Created on 30. listopad 2011, 22:42
 */

#include "GameManager.h"
#include "TerrainBuilder.h"

GameManager::GameManager()
{
    mRootNode = NULL;
    mAspectRatio = 1.0f;
}

GameManager::~GameManager()
{
}

/**
 * Called to update the display.
 * You should call glutSwapBuffers after all of your rendering to display what you rendered.
 * If you need continuous updates of the screen, call glutPostRedisplay() at the end of the function.
 */
void GameManager::OnDisplay()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GameManager * gm = GameManager::getInstance();

    if(gm->mRootNode)
	gm->mRootNode->draw(&gm->mSceneParams);

    glutSwapBuffers();
}

void GameManager::OnReshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    GameManager * gm = GameManager::getInstance();
    gm->mAspectRatio = (float) w / (float) h;
    gm->mSceneParams.projection_mat = glm::perspective(45.f, gm->mAspectRatio, 0.1f, 100.0f);
}

void GameManager::OnTimer(int)
{
    double timed = 0.001 * (double) glutGet(GLUT_ELAPSED_TIME); // milliseconds

    GameManager * gm = GameManager::getInstance();

    if(gm->mRootNode)
	gm->mRootNode->update(timed);

    InputManager::getInstance()->update();
    SwoopManager::getInstance()->update(timed);

    //Detect finished

    glutTimerFunc(33, GameManager::OnTimer, 0);
    glutPostRedisplay();
}

void GameManager::Initialize()
{
    GameManager * gm = GameManager::getInstance();

    glDisable(GL_CULL_FACE); // draw both back and front faces
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    gm->mSceneParams.view_mat = glm::mat4();
    gm->mSceneParams.projection_mat = glm::perspective(
						       45.f, // The horizontal Field of View, in degrees : the amount of "zoom". Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
						       gm->mAspectRatio, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 == 800/600 == 1280/960, sounds familiar ?
						       0.1f, // Near clipping plane. Keep as big as possible, or you'll get precision issues.
						       100.0f // Far clipping plane. Keep as little as possible.
						       );

    /*glm::lookAt(glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	    glm::vec3(0, 0, 0), // and looks at the origin
	    glm::vec3(0, -1, 0) // Head is up (set to 0,-1,0 to look upside-down)
	    );*/

    CameraManager::getInstance()->setSceneParams(&gm->mSceneParams);
    InputManager::Initialize();
    ilInit();
    gm->buildScene();
}

void GameManager::buildScene()
{
    mRootNode = new SceneNode("root");

    TransformNode * skyTrans = new TransformNode("skybox_trans", mRootNode);
    SkyboxNode * skybox = new SkyboxNode("skybox", skyTrans);

    skyTrans->translate(1.0, 0.7, -1.0);
    skyTrans->scale(glm::vec3(20.0));
    //skyTrans->translate(0.0, 0.75, 0.0);
    skybox->loadTexture("data/skybox/");


    //addTerrain();
    CameraManager::getInstance()->createCamera("cam_global", mRootNode);

    // Create terrain
    std::string file = Config::getInstance()->getString("terrain");

    TerrainBuilder tb;
    SceneNode * terrain = tb.prepareNode(mRootNode, "data/desert.tif", "data/testgrid.png", "data/terr01-normals.png");
    tb.loadObjects("data/objects.png"); // XXX objects map via config
    tb.placeObjects();

    // Add Swoop to scene
    SceneNode * swoop = SwoopManager::Initialize();
    swoop->setParentNode(mRootNode);

    // Draw helper line
    TransformNode * lineTrans = new TransformNode("strip-trans", terrain);
    //t->translate(newX, 0, -newY);
    lineTrans->translate(0.0, -0.1, 0.0);
    //    t->scale(glm::vec3(objScale, objScale, objScale));
    new LineStripNode("strip", lineTrans);

    // dump our scene graph tree for debug
    mRootNode->dump();
}

void GameManager::addTerrain()
{


    /*
    // create transformation for the terrain
    TransformNode * ptrans = new TransformNode("terrain-trans", mRootNode);
    ptrans->translate(glm::vec3(0, -0.5, -10));
    //    ptrans->rotate(10.f, glm::vec3(1, 0, 0));
    ptrans->scale(glm::vec3(12, 0.2, 12));
    TerrainNode *terrain = new TerrainNode("terrain", ptrans);
    //terrain->load(file.c_str());
    terrain->load("data/terr01-hmap.png", "data/terr01-normals.png", "data/testgrid.png" );*/

}

void GameManager::Reset()
{
    std::cout << "Resetting scene" << endl;
    GameManager::getInstance()->resetScene();
}

void GameManager::resetScene()
{
    CameraManager::getInstance()->reset();
    SwoopManager::getInstance()->reset();
    delete mRootNode;
    mRootNode = NULL;
    Config::getInstance()->reload();

    buildScene();
}