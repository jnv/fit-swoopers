#version 130

uniform samplerCube cubeMap;
in vec3 texCoord;
//smooth in vec4 theColor;
out vec4 outputColor;
void main()
{
 	outputColor = texture(cubeMap, texCoord);
}
