// Fragment shader for meshes of class MeshNode
// Based on Basic Shading tutorial from http://www.opengl-tutorial.org/
// Released under DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
#version 130

in vec4 theColor;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec2 fragTexCoord;
uniform bool hasTexture;

uniform sampler2D normalMap;
uniform sampler2D colorMap;

uniform struct AmbientLight
{
	vec3 color;
	vec3 position;
	float power;
} AmbientLight0;

out vec3 outputColor;

vec3 sampleNormalBlender()
{
  // 1st we have to invert tex coordinates
  vec4 texel = texture(normalMap, fragTexCoord);
  vec3 ret;
  // now swap coordinates and remap
  ret.x = -texel.x * 2.0 + 1.0; // [0, 1] -> [-1, 1] + invert
  ret.y = texel.z;              // [0, 1] -> [0, 1]
  ret.z = texel.y * 2.0 - 1.0;  // [0, 1] -> [-1, 1]
  return ret;
}

void main()
{
/*
  float k = 1;
  vec3 L = normalize(-thePosition);
  vec3 N = normalize(theNormal);
  outputColor = theColor * dot(N, L);
  */
  	// Light emission properties
	vec3 LightColor = AmbientLight0.color;
	float LightPower = AmbientLight0.power;
	
	// Material properties
	vec3 MaterialDiffuseColor = theColor.rgb;
	
	if(hasTexture)
	{
		MaterialDiffuseColor =  texture(colorMap, fragTexCoord).rgb;
	}
	
	vec3 MaterialAmbiantColor = vec3(0.3) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	// Distance to the light
	float distance = length( AmbientLight0.position - Position_worldspace );

	// Normal of the computed fragment, in camera space
	vec3 n = Normal_cameraspace;
	if(hasTexture)
	{
		//n = sampleNormalBlender();
	}
	n = normalize(n);
	
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );
	// Cosine of the angle between the normal and the light direction, 
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );
	
	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-l,n);
	// Cosine of the angle between the Eye vector and the Reflect vector,
	// clamped to 0
	//  - Looking into the reflection -> 1
	//  - Looking elsewhere -> < 1
	float cosAlpha = clamp( dot( E,R ), 0,1 );
	
	outputColor = 
		// Ambiant : simulates indirect lighting
		MaterialAmbiantColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
		
}
