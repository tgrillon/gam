
#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;

layout(location= 1) in vec2 texcoord;
out vec3 vertex_position;
out vec2 vertex_texcoord;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    
    vertex_position= vec3(mvMatrix * vec4(position, 1));
    vertex_texcoord= texcoord;
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 vertex_position;
in vec2 vertex_texcoord;

uniform sampler2D diffuse_color;

out vec4 fragment_color;

void main( )
{
	vec3 color= vec3(vertex_texcoord, 0);
	
	ivec2 p=  ivec2(vertex_texcoord * 1024);
	if(p.x % 32 < 2)
		color= vec3(1);
	if(p.y % 32 < 2)
		color= vec3(1);
	
	vec3 t= dFdx(vertex_position);
	vec3 b= dFdy(vertex_position);
	vec3 n= normalize(cross(t, b));
	vec3 l= normalize(- vertex_position);
	float cos_theta= max(0, dot(n, l));
	fragment_color= vec4(color * cos_theta, 1);
}

#endif
