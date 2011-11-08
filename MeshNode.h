#ifndef MESH_HODE_H
#define MESH_HODE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SceneNode.h"
#include "util.h"
#include "LightManager.h"

class MeshNode : public SceneNode
{
public:
    /// mesh name is equal to model file name
    MeshNode(const char* file_name = "", SceneNode* parent = NULL);
    ~MeshNode();

    /// load geometry
    bool loadMesh();

    /// reimplemented draw
    void draw(SceneParams * scene_params);

    /// prints the size of the geometry box without transformation
    void printBBoxSize();

protected:

    static GLint m_ViewMatrixLoc;
    static GLint m_ModelMatrixLoc;
    static GLint m_NormalMatrixLoc;

    /// identifier for the program
    static GLuint m_Program;
    /// shader view*model matrix location
    static GLint m_VMmatrixLoc;
    /// shader view matrix location
    static GLint m_ProjectionMatrixLoc;
    /// position attribute location
    static GLint m_PositionLoc;
    /// color attribute location
    static GLint m_ColorLoc;
    /// normal attribute location
    static GLint m_NormalLoc;
    /// identifier for the buffer object
    GLuint m_vertexBufferObject;
    /// count of vertices
    GLuint m_nVertices;

    // the bounding box of the original model
    float minbox[3];
    float maxbox[3];
};

#endif

