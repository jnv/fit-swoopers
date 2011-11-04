#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <GL/glew.h>

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint CreateShader(GLenum eShaderType, const char *filename);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);

#endif
