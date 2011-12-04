/* 
 * File:   SwoopManager.cpp
 * Author: j
 * 
 * Created on 8. listopad 2011, 1:35
 */

#include "SwoopManager.h"
#include "TransformNode.h"

SwoopManager::SwoopManager()
{
    m_inited = false;
    m_finished = false;

    m_waypoints[0] =
	    glm::vec3(0.3, 0, -2.30);
    m_waypoints[1] =
	    glm::vec3(0.42, 0, -1.89);
    m_waypoints[2] =
	    glm::vec3(1.89, 0, -1.5);
    m_waypoints[3] =
	    glm::vec3(2.50, 0, -0.42);
}

SwoopManager::~SwoopManager()
{
}

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

    glm::vec3 closeleft = mesh->getBoxVertex(4);
    glm::vec3 closeright = mesh->getBoxVertex(5);
    glm::vec3 closecenter = closeleft + closeright;
    closecenter /= 2;

    glm::vec3 farleft = mesh->getBoxVertex(7);
    glm::vec3 farright = mesh->getBoxVertex(6);
    glm::vec3 farcenter = farleft + farright;
    farcenter /= 2;

    camera->local->translate(0.0, 0.02, 0.11); //Hand picked... :-P

    new CollidableNode("swoop_collidable", mesh);

    sm->setup();




    //camera->camera->setLocalMatrix(glm::lookAt(closecenter, farcenter, glm::vec3(0, 1, 0)));

    mesh->printBBoxSize();

    return transGlobal;
}

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

void SwoopManager::forward()
{
    //m_transformNode->translate(0, 0, -0.1);
    m_velocity += m_accel_bwd;

}

void SwoopManager::backward()
{
    //m_transformNode->translate(0, 0, 0.1);
    //m_linePos -= m_accel_bwd;
    m_velocity -= m_accel_bwd;
    //smoothstep
}

void SwoopManager::left()
{
    //    m_transformNode->rotate(5, 0, 1, 0);
    if(m_limit_left < m_side_deviation)
    {
	m_transformNode->translate(-m_side_step, 0, 0);
	m_side_deviation -= m_side_step;
    }

    std::cout << m_side_deviation << std::endl;
}

void SwoopManager::right()
{
    //m_transformNode->rotate(-5, 0, 1, 0);
    if(m_side_deviation < m_limit_right)
    {
	m_transformNode->translate(m_side_step, 0, 0);
	m_side_deviation += m_side_step;
    }

    std::cout << m_side_deviation << std::endl;

}

void SwoopManager::update(double time)
{
    if(m_finished)
    {
	return;
    }
    move();

    glm::vec3 newPoint = glm::gtx::spline::catmullRom(m_waypoints[0], m_waypoints[1], m_waypoints[2], m_waypoints[3], m_linePos);

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

void SwoopManager::reset()
{
    m_inited = false;
    m_finished = false;
}

void SwoopManager::finished()
{
    m_finished = true;
}

void SwoopManager::move()
{

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

void SwoopManager::bump()
{
    m_velocity = -m_accel_bwd;
}