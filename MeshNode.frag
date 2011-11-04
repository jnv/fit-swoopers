// Fragment shader for meshes of class MeshNode
#version 130

smooth in vec3 thePosition; // camera space coordinates (-V if light is in camera center)
smooth in vec4 theColor;
smooth in vec3 theNormal; // camera space normal (often denoted N)
uniform vec3 pointlightPos = vec3(0.309363, -0.74156, -4.86074);
uniform vec3 pointlightDir = vec3(0, 1, 0);

out vec4 outputColor;

void main()
{
  float k = 1;
  vec3 L = normalize(thePosition);
  vec3 N = normalize(theNormal);
  outputColor = theColor * dot(N, L);
}
