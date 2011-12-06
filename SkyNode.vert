#version 130
uniform mat4 PVMmatrix;
uniform float texMove;
in vec2 position;
in vec2 texCoord;
smooth out vec2 vTexCoord;

void main()
{
	//gl_Position = PVMmatrix * vec4(position, 0, 1);
	//texCoord = normalize(position.xz);
	gl_Position = PVMmatrix * vec4(position.x, 0, position.y, 1);
	
	vec2 newTexCoord = texCoord + texMove;
	vTexCoord = newTexCoord;
}
