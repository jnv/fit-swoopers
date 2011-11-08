/* 
 * File:   LightManager.cpp
 * Author: j
 * 
 * Created on 8. listopad 2011, 5:12
 */

#include "LightManager.h"

LightManager::LightManager()
{
}

LightManager::~LightManager()
{
}

void LightManager::uniformDirectional(GLuint program)
{
    glUniform3f(glGetUniformLocation(program, "DirectionalLight1.direction"), 0.0f, 1.0f, 11.0f);
    glUniform4f(glGetUniformLocation(program, "DirectionalLight1.ambient"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(program, "DirectionalLight1.diffuse"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(program, "DirectionalLight1.specular"), 1.0f, 1.0f, 1.0f, 1.0f);
}

void LightManager::uniformSpotlight(GLuint program)
{

}