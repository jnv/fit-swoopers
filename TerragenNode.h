#ifndef TERRAGENNODE_H
#define TERRAGENNODE_H


#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include "LightManager.h"
#include "util.h"
#include "SceneNode.h"

/// Generates terrain from Terragen file
class TerragenNode : public SceneNode
{
public:
    TerragenNode(const char * name = "terrain", SceneNode * parent = NULL);

    ~TerragenNode()
    {
    }

    /// load the terrain from file, returns 0 on success
    int load(const char * filename, float scale = 1.0f);

    /// reimplemented draw
    void draw(SceneParams * scene_params);

    int getWidth()
    {
        return 513;// * m_scale;
    }
    float getScale()
    {
        return m_scale;
    }

protected:
    float m_scale;

    /// identifier for the program
    static GLuint m_program;
    /// shader view*model matrix location
    static GLint m_VMmatrixLoc;
    /// shader projection matrix location
    static GLint m_PmatrixLoc;
    /// position attribute location
    static GLint m_posLoc;
    /// color attribute location
    static GLint m_colLoc;

    /// identifier for the buffer object
    GLuint m_vertexBufferObject;
    /// the vertices/color of the terrain in the grid
    float *vertexData;
    /// the number of vertices
    int m_nVertices;
    /// the number of triangles
    int m_nTriangles;
};

#endif // TERRAGENNODE_H