#include "CameraManager.h"

/**
 * Generate arcball vector for given X and Y
 * @param x
 * @param y
 * @return arcball vector
 */
glm::vec3 get_arcball_vector(int x, int y)
{
    int screen_width = glutGet(GLUT_WINDOW_WIDTH);
    int screen_height = glutGet(GLUT_WINDOW_HEIGHT);
    glm::vec3 P = glm::vec3(1.0 * x / screen_width * 2 - 1.0,
			    1.0 * y / screen_height * 2 - 1.0,
			    0);
    P[1] = -P[1];
    float OP_squared = P[0] * P[0] + P[1] * P[1];
    if(OP_squared <= 1 * 1)
	P[2] = sqrt(1 * 1 - OP_squared); // Pythagore
    else
	P = glm::normalize(P); // nearest point
    return P;
}

/**
 * Initializes singleton
 */
CameraManager::CameraManager()
{
    m_current = NULL;
    m_abEnabled = false;
    //m_translate = glm::vec3(0.0);
}

/// NIL, assigned nodes are destroyed by SceneNode
CameraManager::~CameraManager()
{
}

/// Resets manager to empty state
void CameraManager::reset()
{
    clog << "CameraManager::reset" << endl;
    m_current = NULL;
    m_cameras.clear();
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
	m_current->camera->disable();
	++m_current_pos;
	m_current = m_cameras.at(m_current_pos);
    }
    catch(out_of_range& oor)
    {
	std::clog << "CameraManager.nextCamera To front: " << oor.what() << endl;
	m_current = m_cameras.front();
	m_current_pos = 0;
    }

    m_current->camera->activate();
    //m_translate = glm::vec3(0.0);

    std::clog << "Switched to " << m_current->camera->nodeName() << " camera" << endl;

    //recalcView(m_current);
}

/// Creates camera struct, assigns it to parent and stores in vector
CameraStruct* CameraManager::createCamera(const char* name, SceneNode * parent, const bool setActive)
{
    CameraStruct * cs = new CameraStruct(name, parent);

    addCamera(cs, setActive);
    return cs;
}

/**
 * Stores camera struct in vector
 * @param cs struct to store
 * @param setActive whether the added camera should be set as active
 */
void CameraManager::addCamera(CameraStruct* cs, const bool setActive)
{
    m_cameras.push_back(cs);

    std::clog << "Added camera '" << cs->camera->nodeName() << "'" << endl;

    if(m_current == NULL)
    {
	m_current = cs;
	m_current_pos = 0;
	m_current->camera->activate();
    }
    else if(setActive)
    {
	m_current->camera->disable();
	m_current = cs;
	m_current_pos = m_cameras.size() - 1; // Active camera is on last position
	m_current->camera->activate();

    }

}

/**
 * Whether the given camera is active
 * @param camera
 * @deprecated each camera node checks this individually
 * @return
 */
bool CameraManager::isCurrent(CameraNode* camera) const
{
    //return(m_current == camera);
	return false;
}

/// Translates local transform node of current camera
void CameraManager::translate(const glm::vec3& tr)
{
    //scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0, -0.1)) * scene_params.view_mat;
    //    Matrix4f local = m_current->localMatrix();

    //    glm::mat4 local = m_current->localMatrix();
    //    glm::mat4 mtr = glm::gtx::transform::translate(tr);
    //    m_current->setLocalMatrix(local * mtr);

    if(m_current)
    {
	m_current->local->translate(tr);
	//m_translate += tr;
//	std::cout << "(" << m_translate.x << ", " << m_translate.y << ", " << m_translate.z << ")" << std::endl;

    }
}

/// Rotates local transform node of current camera
void CameraManager::rotate(float a, const glm::vec3& av)
{
    //    glm::mat4 local = m_current->localMatrix();
    //    glm::mat4 mro = glm::gtx::transform::rotate(a, av);
    //    m_current->setLocalMatrix(local * mro);
    if(m_current)
    {
	m_current->local->rotate(a, av);
    }
}

/**
 * Recalculates view
 * @deprecated in favor of CameraNode function
 * @param camera
 */
void CameraManager::recalcView(CameraNode* camera)
{

    if(m_abEnabled)
    {
	arcballCalculate();
    }
    else
    {
	m_sceneParams->view_mat = glm::inverse(camera->localMatrix() * camera->globalMatrix());
    }
}

/**
 * Enable arcball handling (usually on mouse click)
 * @param x
 * @param y
 */
void CameraManager::arcballEnable(int x, int y)
{
    m_abEnabled = true;
    m_abLastX = m_abCurX = x;
    m_abLastY = m_abCurY = y;
}

/**
 * Update arcball (on mouse move)
 * @param x
 * @param y
 */
void CameraManager::arcballUpdate(int x, int y)
{
    if(m_abEnabled)
    {
	m_abCurX = x;
	m_abCurY = y;
	arcballCalculate();
    }
}

/// Disable arcball (mouse release)
void CameraManager::arcballDisable()
{
    m_abEnabled = false;
}

/// Recalculates arcball, rotates current camera's local matrix
void CameraManager::arcballCalculate()
{
    if(m_abCurX == m_abLastX && m_abCurY == m_abLastY)
    {
	return;
    }

    glm::vec3 va = get_arcball_vector(m_abLastX, m_abLastY);
    glm::vec3 vb = get_arcball_vector(m_abCurX, m_abCurY);
    float angle = acos(min(1.0f, glm::dot(va, vb)));
    glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
    glm::mat3 camera2object = glm::inverse(glm::mat3(m_current->local->localMatrix()) * glm::mat3(m_current->local->globalMatrix()));
    //    glm::mat3 camera2object = glm::inverse(glm::mat3() * glm::mat3(mesh.object2world));
    glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
    ;
    //    mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);
    //glm::mat4 viewm = glm::rotate(m_sceneParams->view_mat, glm::degrees(angle), axis_in_object_coord);
    ;
    m_current->local->rotate(glm::degrees(angle), axis_in_object_coord);
   // m_current->local->setLocalMatrix(viewm);
    //m_sceneParams->view_mat = viewm;
    m_abLastX = m_abCurX;
    m_abLastY = m_abCurY;
}