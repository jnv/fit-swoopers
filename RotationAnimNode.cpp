#include "RotationAnimNode.h"


RotationAnimNode::RotationAnimNode(const char* name, SceneNode* parent):
  SceneNode(name, parent), m_axis(1, 0, 0), m_angle(0), m_speed(0)
{
}

void RotationAnimNode::update(double elapsed_time)
{
  m_angle = m_speed * elapsed_time;
  m_local_mat = Matrix4f::FromRotation(m_angle, m_axis);

  /// call inherited update (which calculates global matrix and updates children)
  SceneNode::update(elapsed_time);
}
