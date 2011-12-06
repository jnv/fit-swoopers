
#include "TransformNode.h"

TransformNode::TransformNode(const char *name, SceneNode *parent) :
SceneNode(name, parent)
{
}

void TransformNode::setIdentity()
{
    m_local_mat = glm::mat4(1.0f);
}

void TransformNode::translate(const glm::vec3 &tr)
{
    glm::mat4 trmat = glm::gtx::transform::translate(tr);
    m_local_mat *= trmat;
}
void TransformNode::translate(const float x, const float y, const float z)
{

    glm::mat4 trmat = glm::gtx::transform::translate(x, y, z);
    m_local_mat *= trmat;
}

void TransformNode::rotate(float a, const glm::vec3 &vec)
{
    glm::mat4 trmat = glm::gtx::transform::rotate(a, vec);
    m_local_mat *= trmat;
}

void TransformNode::rotate(float a, const float x, const float y, const float z)
{
    glm::mat4 trmat = glm::gtx::transform::rotate(a, x, y, z);
    m_local_mat *= trmat;
}

void TransformNode::scale(const glm::vec3 &sc)
{
    glm::mat4 trmat = glm::gtx::transform::scale(sc);
    m_local_mat *= trmat;
    //    m_local_mat.scale(sc);
}

void TransformNode::scale(const float x, const float y, const float z)
{
    glm::mat4 trmat = glm::gtx::transform::scale(glm::vec3(x, y, z));
    m_local_mat *= trmat;
    //    m_local_mat.scale(sc);
}

void TransformNode::debug() const
{
    //    m_local_mat.dump();
    //std::cout << std::endl;
}
