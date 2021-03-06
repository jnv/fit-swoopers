
#include "LightManager.h"

///Initialize lights
LightManager::LightManager()
{
    m_pointlightNode = NULL;
    m_pointlightGlobalPos = glm::vec4(0);
    m_directionalPos = glm::vec3(1.5, 4.0, -4.0);
    m_directionalColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
    m_directionalEnabled = true;
    m_lightPos = glm::vec4(0, 7, -5, 1.0);
    m_lightColor = glm::vec3(1, 1, 1);
    m_lightEnabled = true;
}

LightManager::~LightManager()
{
}

/// Uniforms ambient light for given shader program
void LightManager::uniformAmbient(GLuint program)
{
    glUniform3f(glGetUniformLocation(program, "AmbientLight0.position"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
    glUniform3f(glGetUniformLocation(program, "AmbientLight0.color"), m_lightColor.x, m_lightColor.y, m_lightColor.z);
    glUniform1f(glGetUniformLocation(program, "AmbientLight0.power"), 0.1f);
    glUniform1i(glGetUniformLocation(program, "AmbientLight0.enabled"), m_lightEnabled);
}

/// Uniforms sotlight for given a shader program
void LightManager::uniformSpotlight(GLuint program)
{
	glUniform1i(glGetUniformLocation(program, "Spotlight1.enabled"), m_lightEnabled);
	glUniform1f(glGetUniformLocation(program, "Spotlight1.range"), 8.0);
	glUniform1f(glGetUniformLocation(program, "Spotlight1.cosInnerCone"), glm::cos(4.0f));
	glUniform1f(glGetUniformLocation(program, "Spotlight1.cosOuterCone"), glm::cos(9.0f));
	glUniform3f(glGetUniformLocation(program, "Spotlight1.position"), m_lightPos.x, m_lightPos.y, m_lightPos.z);
	glUniform3f(glGetUniformLocation(program, "Spotlight1.direction"), 0.0f, 0.0f, -1.0f);
	glUniform4f(glGetUniformLocation(program, "Spotlight1.ambient"), 0.3f, 0.3f, 0.3f, 0.3f);
	glUniform4f(glGetUniformLocation(program, "Spotlight1.diffuse"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4f(glGetUniformLocation(program, "Spotlight1.specular"), 0.5f, 0.5f, 0.5f, 0.5f);
}

/// Uniforms directional light for given shader program
void LightManager::uniformDirectional(GLuint program)
{
    glUniform1i(glGetUniformLocation(program, "directionalLight_enabled"), m_directionalEnabled);
    glUniform3f(glGetUniformLocation(program, "directionalLight_position"), m_directionalPos.x, m_directionalPos.y, m_directionalPos.z);
    glUniform4f(glGetUniformLocation(program, "directionalLight_color"), m_directionalColor.r, m_directionalColor.g, m_directionalColor.b, m_directionalColor.a);

	glUniform3f(glGetUniformLocation(program, "DirectionalLight1.direction"), 0.0, -1.0, 1.0);
	glUniform4f(glGetUniformLocation(program, "DirectionalLight1.ambient"), m_directionalColor.r, m_directionalColor.g, m_directionalColor.b, m_directionalColor.a);
	glUniform4f(glGetUniformLocation(program, "DirectionalLight1.diffuse"), 1.0f, 1.0f, 1.0f, 1.0f);
	glUniform4f(glGetUniformLocation(program, "DirectionalLight1.specular"), 1.0f, 1.0f, 1.0f, 1.0f);
}

/// Recalculate movable light position on update
void LightManager::update()
{
    if(m_pointlightNode)
    {
	m_lightPos = m_pointlightNode->globalMatrix() * glm::vec4(0, 0, 0, 1);
    }
}
