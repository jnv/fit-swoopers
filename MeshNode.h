#ifndef MESH_HODE_H
#define MESH_HODE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "LightManager.h"
#include "SceneNode.h"
#include "util.h"

class MeshNode : public SceneNode
{
public:
    /// mesh name is equal to model file name
    MeshNode(const char* file_name = "", SceneNode* parent = NULL, const bool = true);
    ~MeshNode();

    /// load geometry
    bool loadMesh();

    void loadTexture(const char *, const char *);

    /// reimplemented draw
    virtual void draw(SceneParams * scene_params);

    /// prints the size of the geometry box without transformation
    void printBBoxSize();

    glm::vec3 getBoxVertex(int) const;

    glm::vec3* getBoxVertices();

protected:
    void constructBox();

    /// identifier for the program
    static GLuint m_program;
    /// shader view*model matrix location
    static GLint m_ModelMatrixLoc;
    /// shader view matrix location
    static GLint m_MVPLoc;
    static GLint m_ViewMatrixLoc;
    /// position attribute location
    static GLint m_posLoc;
    /// color attribute location
    static GLint m_colLoc;
    /// normal attribute location
    static GLint m_norLoc;

    static GLint m_texCoordLoc;
    /// identifier for the buffer object
    GLuint m_vertexBufferObject;
    /// count of vertices
    GLuint m_nVertices;

    static GLint m_hasTexLoc;

    // the bounding box of the original model
    float minbox[3];
    float maxbox[3];

    glm::vec3 m_bbox[8];

    bool m_hasTexture;

    /// normal map
    GLuint m_normal_map;
    /// color map
    GLuint m_color_map;

    static GLint m_normalSamplerLoc;
    static GLint m_colorSamplerLoc;


};

#endif

