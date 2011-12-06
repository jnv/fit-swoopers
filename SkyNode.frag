#version 130
uniform sampler2D colorMap;
uniform float texMove;
smooth in vec2 vTexCoord;
out vec4 outputColor;
void main()
{
	vec4 tex = texture2D(colorMap, vTexCoord);
	if(tex.a < 0.5)
	{
		discard;
	}
	tex.a += 0.2;
	
	outputColor = tex;
}
