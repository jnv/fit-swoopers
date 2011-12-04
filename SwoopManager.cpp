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

    m_wayPoints[0] =
	    glm::vec3(0.3, 0, -2.30);
    m_wayPoints[1] =
	    glm::vec3(0.42, 0, -1.89);
    m_wayPoints[2] =
	    glm::vec3(1.89, 0, -1.5);
    m_wayPoints[3] =
	    glm::vec3(2.50, 0, -0.42);
}

SwoopManager::~SwoopManager()
{
}

TransformNode* SwoopManager::Initialize()
{
    string model = Config::getInstance()->getString("swoop_model");
    float swoopY = Config::getInstance()->getFloat("swoop_elevation");


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
    sm->m_lastPos = transGlobal->globalMatrix() * glm::vec4(0.0);
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



    //camera->camera->setLocalMatrix(glm::lookAt(closecenter, farcenter, glm::vec3(0, 1, 0)));

    mesh->printBBoxSize();

    return transGlobal;
}

void SwoopManager::forward()
{
    //m_transformNode->translate(0, 0, -0.1);
    if(m_linePos < 1.0)
    {
	m_linePos += 0.01;
    }

}

void SwoopManager::backward()
{
    //m_transformNode->translate(0, 0, 0.1);
    if(m_linePos > 0.0)
    {
	m_linePos -= 0.01;
	//smoothstep
    }
}

void SwoopManager::left()
{
    //    m_transformNode->rotate(5, 0, 1, 0);
    m_transformNode->translate(-0.1, 0, 0);
}

void SwoopManager::right()
{
    //m_transformNode->rotate(-5, 0, 1, 0);
    m_transformNode->translate(0.1, 0, 0);
}

void SwoopManager::update(double time)
{
    glm::vec3 newPoint = glm::gtx::spline::catmullRom(m_wayPoints[0], m_wayPoints[1], m_wayPoints[2], m_wayPoints[3], m_linePos);

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
    m_lastPos = newGlobPoint;
    m_lastPoint = newPoint;
}

void SwoopManager::reset()
{
    m_inited = false;
    m_finished = false;
}