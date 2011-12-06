#include "SwoopManager.h"


/// Prepares swoop's waypoints

SwoopManager::SwoopManager()
{
    m_inited = false;
    m_finished = false;

    Config * conf = Config::getInstance();

    m_nPoints = conf->getInt("swoop_n_waypoints");

    if(m_nPoints < 4)
    {
	std::cerr << "Please, specify at least 4 waypoints" << std::endl;
    }

    m_points = new glm::vec3[m_nPoints];

    std::clog << "Loading " << m_nPoints << " points" << std::endl;
    for(int i = 0; i < m_nPoints; ++i)
    {
	std::ostringstream oss;
	oss << "swoop_wp" << i;
	m_points[i] = conf->getVec3(oss.str());

	std::clog << m_points[i].x << ", " << m_points[i].y << ", " << m_points[i].z;
    }

}

/**
 * Resets waypoints
 */
SwoopManager::~SwoopManager()
{
    m_nPoints = 0;
    delete [] m_points;
}

/// Prepare swoop's model and related nodes

TransformNode* SwoopManager::Initialize()
{
    Config * conf = Config::getInstance();

    string model = conf->getString("swoop_model");
    float swoopY = conf->getFloat("swoop_elevation");


    SwoopManager * sm = getInstance();

    TransformNode * transGlobal = new TransformNode("swoop_transform_global");
    transGlobal->translate(0, swoopY, 0);

    TransformNode * transLocal = new TransformNode("swoop_transform_local", transGlobal);
    float scale = Config::getInstance()->getFloat("swoop_scale");
    transLocal->scale(glm::vec3(scale, scale, scale));
    transLocal->rotate(-90.f, 0.f, 1.f, 0.f);
    //tnl->translate(glm::vec3(1.0, -0.53, -5.4));

    MeshNode * mesh = new MeshNode(model.c_str(), transLocal);
    mesh->loadMesh();

    sm->m_transformNode = transGlobal;
    sm->m_swoopNode = mesh;
    sm->m_inited = true;
    sm->m_lastPoint = glm::vec3(0.0);
    CameraStruct * camera = CameraManager::getInstance()->createCamera("swoop_cam", transGlobal, true);

    //    glm::vec3 closeleft = mesh->getBoxVertex(4);
    //    glm::vec3 closeright = mesh->getBoxVertex(5);
    //    glm::vec3 closecenter = closeleft + closeright;
    //    closecenter /= 2;
    //
    //    glm::vec3 farleft = mesh->getBoxVertex(7);
    //    glm::vec3 farright = mesh->getBoxVertex(6);
    //    glm::vec3 farcenter = farleft + farright;
    //    farcenter /= 2;

    camera->local->translate(0.0, 0.02, 0.11);

    TransformNode * light = new TransformNode("light", transGlobal);
    LightManager::getInstance()->setPointlightNode(light);
    light->translate(0.0, 0.0, -0.51);


    CollisionManager::getInstance()->makeCollidable(mesh, true);

    bool colDraw = conf->getBool("collidables_draw");
    CollisionManager::getInstance()->setDraw(colDraw);

    sm->setup();

    //camera->camera->setLocalMatrix(glm::lookAt(closecenter, farcenter, glm::vec3(0, 1, 0)));

    mesh->printBBoxSize();

    return transGlobal;
}

/// Load variables from configuration

void SwoopManager::setup()
{
    Config * conf = Config::getInstance();
    m_limit_left = conf->getFloat("swoop_lim_left");
    m_limit_right = conf->getFloat("swoop_lim_right");

    m_accel_fwd = conf->getFloat("swoop_accel_fwd");
    m_accel_bwd = conf->getFloat("swoop_accel_bwd");
    m_side_step = conf->getFloat("swoop_side_step");
    m_decel = conf->getFloat("swoop_decel");
    m_velocity_max_fwd = conf->getFloat("swoop_velocity_max_fwd");
    m_velocity_max_bwd = conf->getFloat("swoop_velocity_max_bwd");
    m_velocity_max_bwd = -m_velocity_max_bwd;
    m_side_deviation = 0;
    m_velocity = 0.0;
}

/// Increment swoop's velocity

void SwoopManager::forward()
{
    //m_transformNode->translate(0, 0, -0.1);
    m_velocity += m_accel_bwd;

}

/// Decrement swoop's velocity

void SwoopManager::backward()
{
    //m_transformNode->translate(0, 0, 0.1);
    //m_linePos -= m_accel_bwd;
    m_velocity -= m_accel_bwd;
    //smoothstep
}

/// Move swoop left

void SwoopManager::left()
{
    //    m_transformNode->rotate(5, 0, 1, 0);
    if(m_limit_left < m_side_deviation)
    {
	m_transformNode->translate(-m_side_step, 0, 0);
	m_side_deviation -= m_side_step;
    }

    //    std::cout << m_side_deviation << std::endl;
}

/// Move swoop right

void SwoopManager::right()
{
    //m_transformNode->rotate(-5, 0, 1, 0);
    if(m_side_deviation < m_limit_right)
    {
	m_transformNode->translate(m_side_step, 0, 0);
	m_side_deviation += m_side_step;
    }

    //    std::cout << m_side_deviation << std::endl;

}

/// Update swoops location depending on its position on Catmull-Rom Spline

void SwoopManager::update(double time)
{
    if(m_finished)
    {
	return;
    }
    m_collides = CollisionManager::getInstance()->hasCollision();

    move();

    glm::vec3 newPoint = glm::gtx::spline::catmullRom(m_points[0], m_points[1], m_points[2], m_points[3], m_linePos);
    //glm::vec3 newPoint = getSplinePoint(m_linePos);


    if(newPoint == m_lastPoint)
    {
	return;
    }

    glm::vec4 oldGlobPoint = m_transformNode->globalMatrix() * glm::vec4(m_lastPoint, 1.0);
    glm::vec4 newGlobPoint = m_transformNode->globalMatrix() * glm::vec4(newPoint, 1.0);

    glm::vec4 delta = newGlobPoint - oldGlobPoint;

    //    std::cout << "Point:" << newPoint.x << "," << newPoint.y << "," << newPoint.z << std::endl;
    //    std::cout << "oldGlobPoint:" << oldGlobPoint.x << "," << oldGlobPoint.y << "," << oldGlobPoint.z << std::endl;
    //    std::cout << "Global:" << newGlobPoint.x << "," << newGlobPoint.y << "," << newGlobPoint.z << std::endl;
    //    std::cout << "Delta: " << delta.x << "," << delta.y << "," << delta.z << std::endl;


    m_transformNode->translate(glm::vec3(delta));
    m_lastPoint = newPoint;
}

/// Clean up

void SwoopManager::reset()
{
    m_inited = false;
    m_finished = false;
    CollisionManager::getInstance()->reset();
}

/// Called once swoop has reached 1.0 on spline

void SwoopManager::finished()
{
    m_finished = true;
}

/// Swoop's movement on spline based on current velocity

void SwoopManager::move()
{
    if(m_collides)
    {
	bump();
    }


    if(m_linePos >= 0.0)
    {
	m_linePos += m_velocity;
    }
    else
    {
	m_linePos = 0.0;
    }

    if(m_velocity > 0.0)
    {
	m_velocity -= m_decel;
    }
    else if(m_velocity < 0.0)
    {
	m_velocity += m_decel;
    }

    if(fabs(m_velocity) < m_decel)
    {
	m_velocity = 0.0;
    }

    if(m_velocity < m_velocity_max_bwd)
    {
	m_velocity = m_velocity_max_bwd;
    }
    else if(m_velocity > m_velocity_max_fwd)
    {
	m_velocity = m_velocity_max_fwd;
    }

    //    m_velocity = glm::clamp(m_velocity, m_velocity_max_bwd, m_velocity_max_fwd);

    if(m_linePos >= 1.0)
    {
	finished();
    }
}


/// Called when swoop collides with something

void SwoopManager::bump()
{
    m_velocity = -m_accel_bwd;
}

glm::vec3 SwoopManager::getSplinePoint(const float pos) const
{
    int index = ((int) glm::floor(pos * (m_nPoints - 3))) + 1; // get the nearest point corresponding to current pos

    return glm::gtx::spline::catmullRom(m_points[index - 1], m_points[index], m_points[index + 1], m_points[index + 2], pos);

}