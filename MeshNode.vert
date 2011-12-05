// Vertex shader for meshes of class MeshNode
// Based on Basic Shading tutorial from http://www.opengl-tutorial.org/
// Released under DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#version 130

uniform mat4 M;
uniform mat4 V;
uniform mat4 MVP;
uniform vec3 LightPosition_worldspace;

in vec3 position;
in vec4 color;
in vec3 normal;
in vec2 texCoord;

out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec4 theColor;
out vec2 fragTexCoord;

void main()
{
/*
	mat4 MV = V*M;
  vec4 pos_v = MV * vec4(position, 1);
  theColor = color;
  theNormal = (MV * vec4(normal, 0)).xyz;
  thePosition = pos_v.xyz / pos_v.w;
  gl_Position = MVP * pos_v;
*/

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(position,1);
	
	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(position,1)).xyz;
	
	// Vector that goes from the vertex to the camera, in camera space.
	// In camera space, the camera is at the origin (0,0,0).
	vec3 vertexPosition_cameraspace = ( V * M * vec4(position,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	Normal_cameraspace = ( V * M * vec4(normal,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	
	// UV of the vertex. No special space for this one.
	theColor = color;
	fragTexCoord = texCoord;
}
