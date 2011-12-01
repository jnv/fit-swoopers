// Vertex shader for heightmapped terrain
#version 130

uniform mat4 Mmatrix;
uniform mat4 Vmatrix;
uniform mat4 Pmatrix;
uniform sampler2D heightMap;
uniform int imgWidth;

uniform float heightCoef;

smooth out vec3 thePosition; // camera space coordinates
smooth out vec2 texCoord; // generated texture coordinates

// terrain is rendered as a set of tiles covering the heightmap
// each tile covers 4 pixels and is made of 6 vertices:
// 0 3---5
// |\ \  |
// | \ \ |
// |  \ \|
// 1---2 4
// vertex coordinates within a tile
float vtxInTilePos[12] = float[](
  0, 1, // 0
  0, 0, // 1
  1, 0, // 2
  0, 1, // 3
  1, 0, // 4
  1, 1  // 5
);

void main()
{
  // we use built-in variable gl_VertexID to figure out where on the map is this vertex
  int tile = gl_VertexID / 6; // id of a tile this vertex belongs to
  int vtx = gl_VertexID % 6; // vertex id within a tile
  // fetch local (in tile) position from our array
  vec2 vtx_pos = vec2(vtxInTilePos[vtx * 2], vtxInTilePos[vtx * 2 + 1]);
  // calculate this tile position
  vec2 tile_pos = vec2(tile % imgWidth, tile / imgWidth);
  // final coordinates are calculated as tile coordinates + local (in tile) coordinates
  // also final coords are normalized to [0.0, 1.0] interval
  texCoord = (tile_pos + vtx_pos) * 1.0 / imgWidth;

  float height = texture(heightMap, texCoord).r;
  //height = height / heightCoef;
  // x and z coordinates match our normalized coordinates, y coord (height) is obtained from heightmap
  vec4 pos_v = Mmatrix * vec4(texCoord.x, height, -texCoord.y, 1);
  // we send our world-space positon to pixel shaders as usual
  thePosition = pos_v.xyz / pos_v.w;
  // we send our camera-perspective coords to opengl
  gl_Position = Vmatrix * Pmatrix * pos_v;
}
