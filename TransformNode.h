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
    /// translate this node by a bunch of floats
    void translate(const float, const float, const float);
    /// rotate this node by angle a along vector vec
    void rotate(float a, const glm::vec3 & vec);
    /// rotate node using a bunch of floats - handy shortcut@
    void rotate(float a, const float, const float, const float);
    /// scale this and children nodes
    void scale(const glm::vec3 & sc);
    /// scale this node by a bunch of floats
    void scale(const float x, const float y, const float z);
    /// debug the matrix
    void debug() const;
};

#endif // of __TRANSFORMNODE_H
