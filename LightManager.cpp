
#include "LightManager.h"

LightManager::LightManager()
{
    m_pointlightNode = NULL;
    m_pointlightGlobalPos = glm::vec4(0);
    m_directionalPos = glm::vec3(1.5, 4.0, -4.0);
    m_directionalColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
    m_directionalEnabled = true;
    m_lightPos = glm::vec4(0, 7, -5, 1.0);
    m_lightColor = glm::vec3(1, 1, 1);
}

LightManager::~LightManager()
{
}

void LightManager::uniformAmbient(GLuint program)
{
    glUniform3f(glGetUniformLocation(program, "AmbientLight0.position"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
    glUniform3f(glGetUniformLocation(program, "AmbientLight0.color"), m_lightColor.x, m_lightColor.y, m_lightColor.z);
    glUniform1f(glGetUniformLocation(program, "AmbientLight0.power"), 0.1f);
}

void LightManager::uniformSpotlight(GLuint program)
{

}

void LightManager::uniformDirectional(GLuint program)
{
    glUniform1i(glGetUniformLocation(program, "directionalLight_enabled"), m_directionalEnabled);
    glUniform3f(glGetUniformLocation(program, "directionalLight_position"), m_directionalPos.x, m_directionalPos.y, m_directionalPos.z);
    glUniform4f(glGetUniformLocation(program, "directionalLight_color"), m_directionalColor.r, m_directionalColor.g, m_directionalColor.b, m_directionalColor.a);
}

void LightManager::update()
{
    if(m_pointlightNode)
    {
	m_lightPos = m_pointlightNode->globalMatrix() * glm::vec4(0, 0, 0, 1);
    }
}