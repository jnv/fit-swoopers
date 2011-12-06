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

/**
 * If this camera is active, store inversion of globalMatrix as view matrix
 * @param scene_params
 */
void CameraNode::draw(SceneParams * scene_params)
{
    if(!m_active)
    {
	return;
    }

    scene_params->view_mat = glm::inverse(globalMatrix());

}

/// Not implemented
void CameraNode::lookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up)
{

}