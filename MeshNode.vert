// Vertex shader for meshes of class MeshNode

#version 130

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ModelMatrix;

in vec3 VertexPosition;
in vec3 VertexNormal;
in vec4 VertexColor;

uniform struct DirectionalLight
{
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} DirectionalLight1;

/*
smooth out vec3 thePosition; // camera space coordinates
smooth out vec4 theColor;
smooth out vec3 Normal; // camera space normal
*/
out vec3 VertexPos;
out vec3 LightDir;
out vec3 Normal;
out vec4 Color;

/*
void main()
{
  vec4 pos_v = VMmatrix * vec4(position, 1);
  theColor = color;
  theNormal = (VMmatrix * vec4(normal, 0)).xyz;
  thePosition = pos_v.xyz / pos_v.w;
  gl_Position = Pmatrix * pos_v;
}
*/
/*
void main()
{
	// All vertex shaders should write the transformed homogeneous clip space
	// vertex position into the gl_Position variables.


	mat4 modelViewMatrix = ViewMatrix * ModelMatrix;
    mat4 mvpMatrix = ProjectionMatrix * modelViewMatrix;
    vec4 pos = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);
    
    gl_Position = mvpMatrix * pos;
    
    // Transform the vertex position into eye space. We use this later on to
    // calculate the view (eye) vector.

    pos = modelViewMatrix * pos;
	VertexPos = pos.xyz / pos.w;
        
    // Transform the light direction into eye space. Directional lights are
    // specified in world space. For example, a directional light aimed along
    // the world negative z axis has the direction vector (0, 0, -1).

	LightDir = vec3(ViewMatrix * vec4(-DirectionalLight1.direction, 0.0f));
    
	//mat3 normalMatrix = transpose(inverse(mat3(modelViewMatrix)));
    //Normal = normalMatrix * VertexNormal;
    Normal = (modelViewMatrix * vec4(VertexNormal, 0)).xyz;
    Color = VertexColor;
}*/

void main()
{
	mat4 modelViewMatrix = ViewMatrix * ModelMatrix;
	mat4 mvpMatrix = ProjectionMatrix * modelViewMatrix;
	vec4 pos = vec4(VertexPosition.x, VertexPosition.y, VertexPosition.z, 1.0);
    gl_Position = mvpMatrix * pos;
    
    Normal = (modelViewMatrix * vec4(VertexNormal, 0)).xyz;
    VertexPos = pos.xyz / pos.w;
    LightDir = vec3(ViewMatrix * vec4(-DirectionalLight1.direction, 0.0f));
}
