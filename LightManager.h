/* 
 * File:   LightManager.h
 * Author: j
 *
 * Created on 8. listopad 2011, 5:12
 */

#ifndef LIGHTMANAGER_H
#define	LIGHTMANAGER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/core/type.hpp>
#include "Singleton.h"
#include "SceneNode.h"

/// Singleton for dumb light management
class LightManager : public Singleton<LightManager>
{
public:
    friend class Singleton<LightManager>;
    LightManager();
    ~LightManager();

    void uniformAmbient(GLuint);
    void uniformSpotlight(GLuint);
    void uniformDirectional(GLuint);

    void uniformPointlight(GLuint);

    void setPointlightNode(SceneNode * node)
    {
        m_pointlightNode = node;
    }

    void switchLight()
    {
        m_lightEnabled = !m_lightEnabled;
    }

    void update();

    bool m_lightEnabled;


protected:

    SceneNode * m_pointlightNode;
    glm::vec4 m_pointlightGlobalPos;
    glm::vec3 m_directionalPos;
    glm::vec4 m_directionalColor;
    glm::vec4 m_lightPos;
    glm::vec3 m_lightColor;
    bool m_directionalEnabled;


private:

};

#endif	/* LIGHTMANAGER_H */

