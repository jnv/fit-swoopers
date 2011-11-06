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

    m_camera_manager = CameraManager::getInstance();
    m_camera_manager->addCamera(this);
}

CameraNode::~CameraNode()
{
}

void CameraNode::update(double elapsed_time)
{
    m_camera_manager->sceneUpdate(this);

    //SceneNode::update(elapsed_time);
}