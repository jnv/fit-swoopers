#include <vector>

#include "CameraManager.h"

/**
 * Just initializes m_current
 */
CameraManager::CameraManager()
{
    m_current = NULL;
}

/// NIL, assigned nodes are destroyed by SceneNode

CameraManager::~CameraManager()
{
}

/**
 * Cycles through cameras
 */
void CameraManager::nextCamera()
{
    if(m_cameras.empty())
    {
	std::cerr << "No CameraNode was added to manager" << endl;
	return;
    }

    try
    {
	++m_current_pos;
	m_current = m_cameras.at(m_current_pos);
    }
    catch(out_of_range& oor)
    {
	std::clog << "CameraManager.nextCamera To front: " << oor.what() << endl;
	m_current = m_cameras.front();
	m_current_pos = 0;
    }

    std::clog << "Switched to " << m_current->nodeName() << " camera" << endl;
}

/**
 * Registers a given camera
 * @param camera
 */
void CameraManager::addCamera(SceneNode* camera)
{
    m_cameras.push_back(camera);

    std::clog << "Added camera '" << camera->nodeName() << "'" << endl;

    if(m_current == NULL)
    {
	m_current = camera;
	m_current_pos = 0;
    }
}

/**
 * Whether the given camera is active
 * @param camera
 * @return
 */
bool CameraManager::isCurrent(SceneNode* camera)
{
    return(m_current == camera);
}

void CameraManager::sceneUpdate(SceneNode* camera)
{
    if(!isCurrent(camera))
	return;

    //m_scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0.1, 0)) * scene_params.view_mat;
}