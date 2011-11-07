#ifndef __SCENE_PARAMS_H
#define __SCENE_PARAMS_H

#include <glm/glm.hpp>

struct SceneParams
{
  glm::mat4 projection_mat;
  glm::mat4 view_mat;
};

#endif // of __SCENE_PARAMS_H