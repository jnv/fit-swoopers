#version 130
uniform mat4 PVM;
in vec3 inPosition;
void main()
{
	gl_Position = PVM * vec4(inPosition, 1.0);
}
