#version 130
uniform mat4 PVMmatrix;
in vec3 position;
void main()
{
	vec3 p = position / 513.f;
	gl_Position = PVMmatrix * vec4(position, 1.0);
}