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

/// Node, that draws pyramid from task3

class TerragenNode : public SceneNode
{
public:
    TerragenNode(const char * name = "terrain", SceneNode * parent = NULL);

    ~TerragenNode()
    {
    }

    /// load the terrain from file, returns 0 on success
    int load(const char * filename);

    /// reimplemented draw
    void draw(SceneParams * scene_params);

    int getWidth()
    {
        return 513;
    }

protected:
    /// identifier for the program
    static GLuint m_program;
    /// shader view*model matrix location
    static GLint m_ModelMatrixLoc;
    /// shader projection matrix location
    static GLint m_MVPLoc;
    static GLint m_ViewMatrixLoc;
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