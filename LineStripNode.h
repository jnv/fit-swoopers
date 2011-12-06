#ifndef LINESTRIPNODE_H
#define LINESTRIPNODE_H

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "SceneNode.h"
#include "util.h"

/// Helper class to a line in scene
class LineStripNode : public SceneNode
{
public:
  LineStripNode(const char * name = "linestrip", SceneNode * parent = NULL);
  ~LineStripNode() {}

  /// reimplemented draw
  void draw(SceneParams * scene_params);

protected:
  /// identifier for the buffer object
  static GLuint m_vertexBufferObject;
  /// identifier for the program
  static GLuint m_program;
  /// shader matrix location
  static GLint m_PVMmatrixLoc;
  /// position attribute location
  static GLint m_posLoc;
  /// color attribute location
  static GLint m_colLoc;
};

#endif

