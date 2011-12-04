#ifndef COLLIDABLENODE_H
#define	COLLIDABLENODE_H

#include "util.h"
#include "SceneNode.h"
#include "MeshNode.h"

class CollidableNode : public SceneNode
{
public:
    CollidableNode(const char * name = "collidable", MeshNode * parent = NULL);
    ~CollidableNode();

    void draw(SceneParams *);

protected:
    MeshNode * m_mesh;
    glm::vec3 * m_vertices;

    /// identifier for the buffer object
    static GLuint m_vertexBufferObject;
    /// identifier for the program
    static GLuint m_program;
    /// shader matrix location
    static GLint m_PVMmatrixLoc;
    /// position attribute location
    static GLint m_posLoc;

private:

};

#endif	/* COLLIDABLENODE_H */

