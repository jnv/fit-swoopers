#ifndef TERRAINNODE_H
#define TERRAINNODE_H

#include <GL/glew.h>
#include "SceneNode.h"

/// Node, that draws pyramid from task3
class TerrainNode : public SceneNode
{
public:
  TerrainNode(const char * name = "terrain", SceneNode * parent = NULL);
  ~TerrainNode() {}

  /// load the terrain from file, returns 0 on success
  int load(const char * hmap, const char * nmap, const char * cmap);

  /// reimplemented draw
  void draw(SceneParams *);

protected:
  /// identifier for the program
  static GLuint m_program;
  /// shader view matrix location
  static GLint m_VmatrixLoc;
  /// shader model matrix location
  static GLint m_MmatrixLoc;
  /// shader projection matrix location
  static GLint m_PmatrixLoc;
  /// shader texture height location
  static GLint m_texSamplerLoc;
  /// shader texture normal location
  static GLint m_normalSamplerLoc;
#if 1
  /// shader texture color location
  static GLint m_colorSamplerLoc;
#endif
  /// width uniform location
  static GLint m_widthLoc;

  static GLint m_heightCoefLoc;

  /// height map
  GLuint m_height_map;
  /// normal map
  GLuint m_normal_map;
  /// color map
  GLuint m_color_map;
  /// height map width
  int m_width;
  /// the number of vertices
  int m_nVertices;
};

#endif // TERRAINNODE_H


