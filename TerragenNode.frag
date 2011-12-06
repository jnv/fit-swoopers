// Fragment shader for meshes of class MeshNode
#version 130

smooth in vec3 thePosition; // camera space coordinates (-V if light is in camera center)
smooth in vec4 theColor;
smooth in vec3 theNormal; // camera space normal (often denoted N)

out vec4 outputColor;

uniform bool directionalLight_enabled;
uniform vec3 directionalLight_position;
uniform vec4 directionalLight_color;

void main()
{
  vec3 N = normalize(theNormal);
  vec3 lightPos = vec3(1, 0.5, -2);
  vec3 spotlightDir = vec3(-1,-1,0);
  vec3 E = -(theNormal.xyz);

  float pointl = dot(normalize(-thePosition), N);

  /*if (dot(normalize(thePosition-light1),normalize(axis1)) > 0.707){
    svetlo -= dot(normalize(-thePosition),normalize(axis1));
  }*/
  
  vec3 L = normalize(directionalLight_position - thePosition);

  outputColor = theColor * clamp(dot(N, L), 0, 1);
  
  //if spotlight enabled
  vec3 lightDir = (lightPos - thePosition.xyz).xyz;
  
  float spotEffect = dot(normalize(spotlightDir), normalize(-lightDir));
  if(spotEffect > 0.707)
  {
  	vec3 HV = normalize(L + E);
  	float NdotHV = max(dot(N, HV), 0.0);
  	//outputColor += vec4(1.0);
  }

  

  /*if (dot(normalize(thePosition-light2),normalize(axis2)) > 0.707){
    svetlo = svetlo - dot(N,normalize(axis2));
  }*/
  
  
}

