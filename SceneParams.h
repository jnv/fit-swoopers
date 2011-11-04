#ifndef __SCENE_PARAMS_H
#define __SCENE_PARAMS_H

#include "Matrix4.h"
#include "Vec3.h"

struct SceneParams
{
  Matrix4f projection_mat;
  Matrix4f view_mat;
};

#endif // of __SCENE_PARAMS_H