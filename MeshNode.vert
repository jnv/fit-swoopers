// Vertex shader for meshes of class MeshNode
#version 130

uniform mat4 VMmatrix;
uniform mat4 Pmatrix;
in vec3 position;
in vec4 color;
in vec3 normal;

smooth out vec3 thePosition; // camera space coordinates
smooth out vec4 theColor;
smooth out vec3 theNormal; // camera space normal

void main()
{
  vec4 pos_v = VMmatrix * vec4(position, 1);
  theColor = color;
  theNormal = (VMmatrix * vec4(normal, 0)).xyz;
  thePosition = pos_v.xyz / pos_v.w;
  gl_Position = Pmatrix * pos_v;
}
