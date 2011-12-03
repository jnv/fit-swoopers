/* 
 * File:   GameManager.h
 * Author: j
 *
 * Created on 30. listopad 2011, 22:42
 */

#ifndef GAMEMANAGER_H
#define	GAMEMANAGER_H

#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <IL/il.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Singleton.h"
#include "Config.h"
#include "util.h"
#include "PyramidNode.h"
#include "TransformNode.h"
#include "RotationAnimNode.h"
#include "TerrainNode.h"
#include "MeshNode.h"
#include "SceneParams.h"
#include "CameraManager.h"
#include "CameraNode.h"
#include "LineStripNode.h"
#include "InputManager.h"
#include "SwoopManager.h"

class GameManager : public Singleton<GameManager>
{
public:
    friend class Singleton<GameManager>;
    SceneParams mSceneParams;
    SceneNode * mRootNode; // scene root
    float mAspectRatio;

    static void Initialize();
    static void OnDisplay();
    static void OnTimer(int);
    static void OnReshape(int, int);

    static void Reset();


protected:

    void buildScene();
    void addTerrain();
    void resetScene();

private:
    GameManager();
    virtual ~GameManager();

};

#endif	/* GAMEMANAGER_H */

