#ifndef SKYNODE_H
#define	SKYNODE_H

#include "SceneNode.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "util.h"

/**
 * Creates plane with animated teture (movement is based on m_move)
 */
class SkyNode : public SceneNode
{
public:
    SkyNode(const char * = "sky", SceneNode * = NULL);
    virtual ~SkyNode();

    bool loadTexture(const char* file);


    //    void update(double);

    void draw(SceneParams*);

protected:
    GLuint m_vertexBufferObject;
    static GLuint m_program;
    static GLint m_PVMmatrixLoc;
    static GLint m_posLoc;
    /// Shader location for m_move
    static GLint m_moveLoc;
    static GLint m_colorSamplerLoc;
    static GLint m_texCoordLoc;
    GLuint m_color_map;

    /// Used for texture movement
    float m_move;
private:

};

#endif	/* SKYNODE_H */

