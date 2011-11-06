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
    try
    {
	++m_current_pos;
	m_current = m_cameras.at(m_current_pos);
    }
    catch(std::out_of_range& oor)
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
