#version 130
uniform mat4 PVMmatrix;
in vec4 position;
in vec4 color;
smooth out vec4 theColor;
void main()
{
	gl_Position = PVMmatrix * position;
	theColor = color;
}