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
}

SwoopManager::~SwoopManager()
{
}

TransformNode* SwoopManager::Initialize(const char * model)
{
    SwoopManager * sm = getInstance();

    TransformNode * tng = new TransformNode("swoop_transform_global");
    TransformNode * tnl = new TransformNode("swoop_transform_local", tng);

    //MeshNode * mn = new MeshNode("data/model/swoop-maya.obj", tnl); //XXX data file via configuration
    MeshNode * mn = new MeshNode(model, tnl); //XXX data file via configuration

    //tnl->translate(glm::vec3(1.0, -0.53, -5.4));
    float scale = Config::getInstance()->getFloat("swoop_scale");
    tnl->scale(glm::vec3(scale, scale, scale));
    tnl->rotate(-90.f, 0.f, 1.f, 0.f);

    mn->loadMesh();

    sm->m_transformNode = tng;
    sm->m_swoopNode = mn;

    return tng;
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