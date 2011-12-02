// Fragment shader for meshes of class MeshNode
#version 130

smooth in vec3 thePosition; // camera space coordinates (-V if light is in camera center)
smooth in vec4 theColor;
smooth in vec3 theNormal; // camera space normal (often denoted N)

out vec4 outputColor;

void main()
{
  vec3 V = normalize(- thePosition);
  vec3 light1 = vec3(0.309363, -0.74156, -4.86074);
  vec3 axis1 = vec3(-1,-1,0);
  vec3 light2 = vec3(0.123297, -0.68428, -5.200);
  vec3 axis2 = vec3(-1,-1,0);

  float svetlo = dot(V, normalize(theNormal));

  if (dot(normalize(thePosition-light1),normalize(axis1)) > 0.707){
    svetlo = svetlo - dot(V,normalize(axis1));
  }

  if (dot(normalize(thePosition-light2),normalize(axis2)) > 0.707){
    svetlo = svetlo - dot(V,normalize(axis2));
  }

  outputColor = theColor;// * svetlo;
 
}
