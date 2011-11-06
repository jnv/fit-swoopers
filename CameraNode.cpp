/* 
 * File:   CameraNode.cpp
 * Author: j
 * 
 * Created on 6. listopad 2011, 11:47
 */

#include "CameraNode.h"

CameraNode::CameraNode(const char* name, SceneNode* parent) :
SceneNode(name, parent)
{

    CameraManager * manager = CameraManager::getInstance();
    manager->addCamera(this);
}

CameraNode::~CameraNode()
{
}

void CameraNode::update(double elapsed_time)
{
    //if active camera in camera manager
    //m_global_mat = inversion of parent's global matrix;

    //SceneNode::update(elapsed_time);
}