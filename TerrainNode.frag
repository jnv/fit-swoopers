// Fragment shader for meshes of class MeshNode
#version 130

smooth in vec3 thePosition; // camera space coordinates (-V if light is in camera center)
smooth in vec2 texCoord;

uniform mat4 Mmatrix;
uniform mat4 Vmatrix;

uniform sampler2D normalMap;

out vec4 outputColor;

vec3 lPos = vec3(0, 0, 0);

// this code samples blender tangent space normalmap to be usable on our terrain
vec3 sampleNormalBlender()
{
  // 1st we have to invert tex coordinates
  vec4 texel = texture(normalMap, vec2(1, 1) - texCoord);
  vec3 ret;
  // now swap coordinates and remap
  ret.x = -texel.x * 2.0 + 1.0; // [0, 1] -> [-1, 1] + invert
  ret.y = texel.z;              // [0, 1] -> [0, 1]
  ret.z = texel.y * 2.0 - 1.0;  // [0, 1] -> [-1, 1]
  return ret;
}

// this code samples blender tangent space normalmap to be usable on our terrain
vec3 sampleNormalGenerated()
{
  // 1st we have to invert tex coordinates
  vec4 texel = texture(normalMap, texCoord);
  vec3 ret;
  // now swap coordinates and remap
  ret.x = texel.x * 2.0 - 1.0;
  ret.y = texel.z;
  ret.z = -texel.y * 2.0 + 1.0;
  return ret;
}

void main()
{
  //vec3 N = sampleNormalBlender();
  vec3 N = vec3(0,1,0);
  N = normalize(N);
  N = (Vmatrix * Mmatrix * vec4(N, 0)).xyz;

  vec3 L = normalize(lPos - thePosition);
  outputColor = vec4(1, 1, 1, 1) * clamp(dot(N, L), 0, 1);
}
