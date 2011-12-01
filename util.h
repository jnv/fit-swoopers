#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <glm/glm.hpp>
#include <GL/glew.h>

struct TextureStruct
{
    unsigned char * data;
    int w;
    int h;
    int size;

    ~TextureStruct()
    {
        delete [] data;
    }
};

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
GLuint CreateShader(GLenum eShaderType, const char *filename);
GLuint CreateProgram(const std::vector<GLuint> &shaderList);

GLuint CreateTexture(const char * filename);
GLuint CreateNormalTexture(const char * filename);
GLuint CreateHeightTexture(const char *, TextureStruct&);

inline void clampf(float &v, const float vmin, const float vmax)
{
    if (v < vmin) v = vmin;
    else if (v > vmax) v = vmax;
}



#endif
