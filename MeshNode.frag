// Fragment shader for meshes of class MeshNode
#version 130

uniform struct DirectionalLight
{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} DirectionalLight1;

in vec3 VertexPos;
in vec3 LightDir;
in vec3 Normal;
in vec4 Color;

/*
smooth in vec3 thePosition; // camera space coordinates (-V if light is in camera center)
smooth in vec4 theColor;
smooth in vec3 theNormal; // camera space normal (often denoted N)
uniform vec3 pointlightPos = vec3(0.309363, -0.74156, -4.86074);
uniform vec3 pointlightDir = vec3(0, 1, 0);
*/

out vec4 FragColor;

/*
void main()
{
  float k = 1;
  vec3 L = normalize(-thePosition);
  vec3 N = normalize(theNormal);
  outputColor = theColor * dot(N, L);
}
*/

void main()
{
	vec3 L = normalize(LightDir);
	vec3 N = normalize(Normal);
	vec3 V = normalize(-VertexPos);
	vec3 R = normalize(-reflect(L, N));

	float nDotL = max(0.0, dot(N, L));
	float rDotV = max(0.0, dot(R, V));
	
	float frag_shininess = 20.0;
	vec4 frag_specular = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 frag_diffuse = vec4(0.8, 0.8, 0.8, 0.0);
	
	vec4 diffuse_factor = nDotL * DirectionalLight1.diffuse;
	vec4 ambient_diffuse_factor = diffuse_factor + DirectionalLight1.ambient;
	vec4 specular_factor = max(pow(-dot(R, V), frag_shininess), 0.0) * DirectionalLight1.specular;
	
	//vec4 ambient = DirectionalLight1.ambient;// * Color; // * Material1.ambient;
	//vec4 diffuse = DirectionalLight1.diffuse * Color * nDotL; //Material1.diffuse * nDotL;
	//vec4 specular = DirectionalLight1.specular * Material1.specular * pow(rDotV, Material1.shininess);
	//vec4 specular = DirectionalLight1.specular * Color;
	//vec4 texel = texture(ColorMapSampler, TexCoord);
	
    //FragColor = ambient + diffuse + specular;
    FragColor = specular_factor * frag_specular + ambient_diffuse_factor * frag_diffuse;
	//FragColor = Color;
}
