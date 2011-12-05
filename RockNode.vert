// Vertex shader for heightmapped terrain
#version 130

uniform mat4 Mmatrix;
uniform mat4 Vmatrix;
uniform mat4 Pmatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec3 vertexPos;
out vec2 fragTexCoord;

void main()
{
	fragTexCoord = texCoord;
	vec4 pos = Pmatrix * Vmatrix * Mmatrix * vec4(position,1.0);
	vertexPos = vec3(pos);
	gl_Position = pos;
}
