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

//void CameraNode::update(double elapsed_time)
//{
//    m_camera_manager->sceneUpdate(this);
//
//    //SceneNode::update(elapsed_time);
//}

void CameraNode::draw(SceneParams * scene_params)
{
    if(m_camera_manager->isCurrent(this))
    {
	glm::mat4 global = globalMatrix();

	glm::mat4 view = scene_params->view_mat;

	glm::mat4 coor = global * view;
//	coor.dump();

	//scene_params->view_mat = globalMatrix();
	scene_params->view_mat = global;
	//	scene_params->projection_mat = newm.inverse();
    }
    //m_scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0.1, 0)) * scene_params.view_mat;
}