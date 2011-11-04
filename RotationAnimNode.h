#ifndef ROTATION_ANIM_NODE_H
#define ROTATION_ANIM_NODE_H

#include "SceneNode.h"
#include "Vec3.h"

class RotationAnimNode : public SceneNode
{
public:
  RotationAnimNode(const char* name = "<unk>", SceneNode* parent = NULL);
  ~RotationAnimNode() {}

  void setSpeed(float speed) { m_speed = speed; }
  void setAxis(const Vec3f & axis) { m_axis = axis; }

  void update(double elapsed_time);

protected:
  Vec3f m_axis;
  float m_angle;
  float m_speed;
};

#endif
