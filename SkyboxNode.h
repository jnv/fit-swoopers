#ifndef SKYBOXNODE_H
#define	SKYBOXNODE_H

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "SceneNode.h"
#include "util.h"

/**
 * Draws a regular cube with skybox texture
 */
class SkyboxNode : public SceneNode
{
public:
    SkyboxNode(const char * = "skybox", SceneNode * = NULL);
    virtual ~SkyboxNode();

    /// reimplemented draw
    void draw(SceneParams * scene_params);

    void loadTexture(const char *);

protected:
    /// identifier for the buffer object
    static GLuint m_vertexBufferObject;
    /// identifier for the program
    static GLuint m_program;
    /// shader matrix location
    static GLint m_PVMmatrixLoc;
    /// position attribute location
    static GLint m_posLoc;
    /// color attribute location
    static GLint m_colLoc;

    static GLuint m_color_map;
private:

};

#endif	/* SKYBOXNODE_H */

