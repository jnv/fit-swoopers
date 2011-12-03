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
    CameraStruct * camera = CameraManager::getInstance()->createCamera("swoop_cam", transGlobal);

    mesh->printBBoxSize();

    return transGlobal;
}

void SwoopManager::forward()
{
    m_transformNode->translate(0, 0, -0.1);
}

void SwoopManager::backward()
{
    m_transformNode->translate(0, 0, 0.1);
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

}

void SwoopManager::reset()
{
    m_inited = false;
}