#include "CameraNode.h"

/// Just call parent's constructor
CameraNode::CameraNode(const char *name, SceneNode *parent) :
SceneNode(name, parent)
{
}

/// nil.
CameraNode::~CameraNode()
{
}

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