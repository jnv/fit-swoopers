// Vertex shader for heightmapped terrain
#version 130

uniform mat4 Mmatrix;
uniform mat4 Vmatrix;
uniform mat4 Pmatrix;

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 vertexPos;
out vec2 TexCoord;

void main()
{
	TexCoord = vec2(texCoord);
	gl_Position = Pmatrix * Vmatrix * Mmatrix * vec4(position,1.0);
}
