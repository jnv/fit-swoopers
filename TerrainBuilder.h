/* 
 * File:   TerrainBuilder.h
 * Author: j
 *
 * Created on 1. prosinec 2011, 23:07
 */

#ifndef TERRAINBUILDER_H
#define	TERRAINBUILDER_H

#include "TerrainNode.h"
#include "TerragenNode.h"
#include "TransformNode.h"
#include "util.h"
#include <IL/il.h>
#include <iostream>
#include <iomanip>
#include "CameraManager.h"
#include "SwoopManager.h"
#include "Config.h"
#include "LineStripNode.h"
#include "RockNode.h"
using namespace std;

class TerrainBuilder
{
public:
    
    TerrainBuilder();
    virtual ~TerrainBuilder();

    SceneNode* prepareNode(SceneNode *, const char *, const char *, const char *);
    bool loadObjects(const char *);
    void placeObjects();

protected:
    //TerrainNode * m_terrainNode;
    TerragenNode * m_terrainNode;
    TransformNode * m_terrainParent;

    void addPyramid(const int, const int);
    void addSwoop(const int, const int);
    void addRock(const int, const int);


private:

};

#endif	/* TERRAINBUILDER_H */

