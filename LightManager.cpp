#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

void LightManager::uniformAmbient(GLuint program)
{
    glm::vec3 lightPos = glm::vec3(0, 7, -5);
    glm::vec3 lightColor = glm::vec3(1, 1, 1);
    glUniform3f(glGetUniformLocation(program, "AmbientLight0.position"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(program, "AmbientLight0.color"), lightColor.r, lightColor.g, lightColor.b);
    glUniform1f(glGetUniformLocation(program, "AmbientLight0.power"), 50.0f);
}

void LightManager::uniformSpotlight(GLuint program)
{

}
