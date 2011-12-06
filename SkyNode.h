/* 
 * File:   SkyNode.h
 * Author: j
 *
 * Created on 5. prosinec 2011, 22:00
 */

#ifndef SKYNODE_H
#define	SKYNODE_H

#include "SceneNode.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "util.h"

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
    static GLint m_moveLoc;
    static GLint m_colorSamplerLoc;
    static GLint m_texCoordLoc;
    GLuint m_color_map;

    float m_move;
private:

};

#endif	/* SKYNODE_H */

