/* 
 * File:   CameraNode.cpp
 * Author: j
 * 
 * Created on 6. listopad 2011, 11:47
 */

#include "CameraNode.h"

CameraNode::CameraNode(const char *name, SceneNode *parent) :
SceneNode(name, parent)
{
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
    //    if(m_camera_manager->isCurrent(this))
    //    {
    //	glm::mat4 global = globalMatrix();
    //	glm::mat4 local = localMatrix();
    //
    //	glm::mat4 view = scene_params->view_mat;
    //
    //	glm::mat4 coor = global * view;
    ////	coor.dump();
    //
    //	//scene_params->view_mat = globalMatrix();
    //	//scene_params->view_mat = glm::inverseTranspose(global);
    //	scene_params->view_mat = global;// * glm::inverseTranspose(view);
    //    }
    //m_scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0.1, 0)) * scene_params.view_mat;
    if(!m_active)
    {
	return;
    }

    //std::cout << "draw: " << nodeName() << std::endl;

    scene_params->view_mat = glm::inverse(globalMatrix());

//    glm::vec3 eye = glm::mat3(globalMatrix()) * glm::vec3(1.0, 1.0, 1.0);

//    scene_params->view_mat = glm::lookAt(
//					 eye, // The eye's position in 3d space
//					 glm::vec3(0.0f, 0.0f, 0.0f), // What the eye is looking at
//					 glm::vec3(0.0f, 1.0f, 0.0f)); // The eye's orientation vector (which way is up)
}