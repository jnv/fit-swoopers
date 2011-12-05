#version 130
uniform mat4 PVMmatrix;
in vec4 position;
out vec3 texCoord;
//in vec4 color;
void main()
{
	texCoord = normalize(position.xyz);
	gl_Position = PVMmatrix * position;
	//theColor = color;
}
