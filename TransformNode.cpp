
#include "TransformNode.h"

TransformNode::TransformNode(const char *name, SceneNode *parent) :
SceneNode(name, parent)
{
}

void TransformNode::setIdentity()
{
    m_local_mat = Matrix4f::Identity();
}

void TransformNode::translate(const Vec3f &tr)
{
    m_local_mat.translate(tr);
}

void TransformNode::rotate(float a, const Vec3f &vec)
{
    m_local_mat.rotate(a, vec);
}

void TransformNode::scale(const Vec3f &sc)
{
    m_local_mat.scale(sc);
}

void TransformNode::invert()
{
    m_local_mat = m_local_mat.inverse();
}

void TransformNode::debug() const
{
    m_local_mat.dump();
    std::cout << std::endl;
}
