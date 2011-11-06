#ifndef MESH_HODE_H
#define MESH_HODE_H

#include <GL/glew.h>
#include "SceneNode.h"

class MeshNode : public SceneNode
{
public:
    /// mesh name is equal to model file name
    MeshNode(const char* file_name = "", SceneNode* parent = NULL);
    ~MeshNode();

    /// load geometry
    bool loadMesh();

    /// reimplemented draw
    void draw(const SceneParams * scene_params);

    /// prints the size of the geometry box without transformation
    void printBBoxSize();

protected:
    /// identifier for the program
    static GLuint m_program;
    /// shader view*model matrix location
    static GLint m_VMmatrixLoc;
    /// shader view matrix location
    static GLint m_PmatrixLoc;
    /// position attribute location
    static GLint m_posLoc;
    /// color attribute location
    static GLint m_colLoc;
    /// normal attribute location
    static GLint m_norLoc;
    /// identifier for the buffer object
    GLuint m_vertexBufferObject;
    /// count of vertices
    GLuint m_nVertices;

    // the bounding box of the original model
    float minbox[3];
    float maxbox[3];
};

#endif

