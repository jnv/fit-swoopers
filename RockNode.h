/* 
 * File:   RockNode.h
 * Author: j
 *
 * Created on 4. prosinec 2011, 23:50
 */

#ifndef ROCKNODE_H
#define	ROCKNODE_H
#include "util.h"
#include "MeshNode.h"

class RockNode : public MeshNode
{
public:
    RockNode(SceneNode* parent = NULL);
    virtual ~RockNode();

    void loadTexture();

    void draw(SceneParams*);

protected:
    /// identifier for the program
    static GLuint m_program;
    /// shader view matrix location
    static GLint m_VmatrixLoc;
    /// shader model matrix location
    static GLint m_MmatrixLoc;
    /// shader projection matrix location
    static GLint m_PmatrixLoc;
    /// shader texture normal location
    static GLint m_normalSamplerLoc;
    /// shader texture color location
    static GLint m_colorSamplerLoc;
    /// width uniform location
    static GLint m_widthLoc;

    /// normal map
    GLuint m_normal_map;
    /// color map
    GLuint m_color_map;


private:

};

#endif	/* ROCKNODE_H */

