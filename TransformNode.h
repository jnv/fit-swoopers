#ifndef __TRANSFORMNODE_H
#define __TRANSFORMNODE_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "SceneNode.h"

/// TransformNode represents transformation of scene graph subtree.

class TransformNode : public SceneNode
{
public:
    TransformNode(const char* name = "<unk>", SceneNode* parent = NULL);

    ~TransformNode()
    {
    }

    /// resets local matrix to identity
    void setIdentity();
    /// translate this node by vector tr
    void translate(const glm::vec3 & tr);
    /// rotate this node by angle a along vector vec
    void rotate(float a, const glm::vec3 & vec);
    /// scale this and children nodes
    void scale(const glm::vec3 & sc);
    // debug the matrix
    void debug() const;

    void invert();
};

#endif // of __TRANSFORMNODE_H
