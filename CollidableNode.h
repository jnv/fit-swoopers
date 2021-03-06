#ifndef COLLIDABLENODE_H
#define	COLLIDABLENODE_H

#include "util.h"
#include "SceneNode.h"
#include "MeshNode.h"

/**
 * Reference node for simple collision testing
 */
class CollidableNode : public SceneNode
{
public:
    CollidableNode(const char * name = "collidable", MeshNode * parent = NULL);
    ~CollidableNode();

    void draw(SceneParams *);

    static void SwitchDraw();
    static void SetDraw(const bool);

    glm::vec3 getGlobalVertex(const int) const;
    glm::vec2 getGlobalVertex2D(const int) const;

    const float left() const;
    const float right() const;
    const float top() const;
    const float bottom() const;
protected:
    /// Ref to parent aka MeshNode, so we keep access to its verticles
    MeshNode * m_mesh;
    /// Local store of parent's verticles
    glm::vec3 * m_vertices;

    /// identifier for the buffer object
    GLuint m_vertexBufferObject;
    /// identifier for the program
    static GLuint m_program;
    /// shader matrix location
    static GLint m_PVMmatrixLoc;
    /// position attribute location
    static GLint m_posLoc;
    /// whether to draw wireframe
    static bool m_draw;

private:

};

#endif	/* COLLIDABLENODE_H */

