#version 330

#ifdef VERTEX_SHADER

layout(location= 0) in vec3 a_Position;
layout(location= 1) in vec2 a_Texcoord;
layout(location= 2) in vec3 a_Normal;

out vec3 vPosition;
out vec2 vTexcoord;
out vec3 vNormal;

uniform mat4 u_MvpMatrix;
uniform mat4 u_MvMatrix;
uniform mat4 u_NormalMatrix;

void main()
{
  gl_Position= u_MvpMatrix * vec4(a_Position, 1);

  vTexcoord= a_Texcoord;
  vNormal= mat3(u_NormalMatrix) * a_Normal;
  vPosition= vec3(u_MvMatrix * vec4(a_Position, 1));
}
#endif

#ifdef FRAGMENT_SHADER

in vec3 vPosition;
in vec2 vTexcoord;
in vec3 vNormal;

out vec4 out_color;

uniform vec3 u_Light;
uniform vec4 u_LightColor= vec4(1, 1, 1, 1);
uniform vec4 u_MeshColor= vec4(1, 1, 1, 1);
uniform bool u_UseCurvature; 

void main( )
{
  vec4 color= u_MeshColor;
  if (u_UseCurvature)
  {
    color= vec4(vTexcoord.xxx, 1.0);
  }

  vec3 normal= normalize(vNormal);

  float cos_theta= max(0, dot(normal, normalize(u_Light - vPosition)));         
  color= color * u_LightColor;
    
  if(gl_FrontFacing == false)
  {
    ivec2 pixel= ivec2(gl_FragCoord.xy / 4) % ivec2(2, 2);
    if((pixel.x ^ pixel.y) == 0)
      color= vec4(0.8, 0.4, 0, 1);
  }
  
  color.rgb= color.rgb * cos_theta;
    
  out_color= color;
}
#endif
