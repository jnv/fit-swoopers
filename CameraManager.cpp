#include "CameraManager.h"

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
 * Just initializes m_current
 */
CameraManager::CameraManager()
{
    m_current = NULL;
    m_abEnabled = false;
    m_translate = glm::vec3(0.0);
}

/// NIL, assigned nodes are destroyed by SceneNode

CameraManager::~CameraManager()
{
}

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
    m_translate = glm::vec3(0.0);

    std::clog << "Switched to " << m_current->camera->nodeName() << " camera" << endl;

    //recalcView(m_current);
}

CameraStruct* CameraManager::createCamera(const char* name, SceneNode * parent)
{
    CameraStruct * cs = new CameraStruct(name, parent);

    addCamera(cs);
    return cs;
}

/**
 * Registers a given camera
 * @param camera
 */
void CameraManager::addCamera(CameraStruct* cs)
{
    m_cameras.push_back(cs);

    std::clog << "Added camera '" << cs->camera->nodeName() << "'" << endl;

    if(m_current == NULL)
    {
	m_current = cs;
	m_current_pos = 0;
	m_current->camera->activate();
    }
}

/**
 * Whether the given camera is active
 * @param camera
 * @return
 */
bool CameraManager::isCurrent(CameraNode* camera) const
{
    //return(m_current == camera);
}

/*
void CameraManager::sceneDraw(CameraNode* camera)
{
    if(!isCurrent(camera))
	return;

    recalcView(camera);

    //m_scene_params.view_mat = Matrix4<float>::FromTranslation(Vec3f(0, 0.1, 0)) * scene_params.view_mat;
}*/

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
	m_translate += tr;
	std::cout << "(" << m_translate.x << ", " << m_translate.y << ", " << m_translate.z << ")" << std::endl;

    }
}

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

void CameraManager::arcballEnable(int x, int y)
{
    m_abEnabled = true;
    m_abLastX = m_abCurX = x;
    m_abLastY = m_abCurY = y;
}

void CameraManager::arcballUpdate(int x, int y)
{
    if(m_abEnabled)
    {
	m_abCurX = x;
	m_abCurY = y;
    }
}

void CameraManager::arcballDisable()
{
    m_abEnabled = false;
}

void CameraManager::arcballCalculate()
{/*
    if(m_abCurX == m_abLastX && m_abCurY == m_abLastY)
    {
	return;
    }

    glm::vec3 va = get_arcball_vector(m_abLastX, m_abLastY);
    glm::vec3 vb = get_arcball_vector(m_abCurX, m_abCurY);
    float angle = acos(min(1.0f, glm::dot(va, vb)));
    glm::vec3 axis_in_camera_coord = glm::cross(va, vb);
    glm::mat3 camera2object = glm::inverse(glm::mat3(m_current->localMatrix()) * glm::mat3(m_current->globalMatrix()));
    //    glm::mat3 camera2object = glm::inverse(glm::mat3() * glm::mat3(mesh.object2world));
    glm::vec3 axis_in_object_coord = camera2object * axis_in_camera_coord;
    ;
    //    mesh.object2world = glm::rotate(mesh.object2world, glm::degrees(angle), axis_in_object_coord);
    glm::mat4 viewm = glm::rotate(m_sceneParams->view_mat, glm::degrees(angle), axis_in_object_coord);
    ;
    m_current->setLocalMatrix(viewm);
    m_sceneParams->view_mat = viewm;
    m_abLastX = m_abCurX;
    m_abLastY = m_abCurY;*/
}