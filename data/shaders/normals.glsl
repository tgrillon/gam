
#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
layout(location= 2) in vec3 normal;

out vec3 vertex_normal;

void main( )
{
    gl_Position= vec4(position, 1);
    vertex_normal= normal;
}
#endif

#ifdef GEOMETRY_SHADER
uniform mat4 mvpMatrix;
uniform mat4 normalMatrix;
uniform float scale = 1;

in vec3 vertex_normal[]; 

out vec3 geometry_color;

layout(triangles) in;
layout(line_strip, max_vertices= 16) out;
void main( )
{
//aretes du triangle
	vec3 ab= gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 ac= gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;

//normale geometrique du triangle
	vec3 gn= normalize(cross(ab, ac));
	
	geometry_color= vec3(0.8, 0.4, 0);
	vec3 c= gl_in[0].gl_Position.xyz + ab / 3 + ac / 3;	// centre du triangle
	gl_Position= mvpMatrix * vec4(c, 1); EmitVertex();
	gl_Position= mvpMatrix * vec4(c + gn * scale / 2, 1); EmitVertex();
	EndPrimitive();
	
	geometry_color= vec3(0.4, 0.8, 0);
// normale en a
	gl_Position= mvpMatrix * gl_in[0].gl_Position; EmitVertex();
	gl_Position= mvpMatrix * vec4(gl_in[0].gl_Position.xyz + vertex_normal[0] * scale, 1); EmitVertex();
	EndPrimitive();
	
// normale en b
	gl_Position= mvpMatrix * gl_in[1].gl_Position; EmitVertex();
	gl_Position= mvpMatrix * vec4(gl_in[1].gl_Position.xyz + vertex_normal[1] * scale, 1); EmitVertex();
	EndPrimitive();
	
// normale en c
	gl_Position= mvpMatrix * gl_in[2].gl_Position; EmitVertex();
	gl_Position= mvpMatrix * vec4(gl_in[2].gl_Position.xyz + vertex_normal[2] * scale, 1); EmitVertex();
	EndPrimitive();
	
	geometry_color= vec3(0.8, 0.4, 0);
// arete ab
	gl_Position= mvpMatrix * vec4(gl_in[0].gl_Position.xyz + gn * 0.005, 1); EmitVertex();
	gl_Position= mvpMatrix * vec4(gl_in[1].gl_Position.xyz + gn * 0.005, 1); EmitVertex();
	EndPrimitive();
	
// arete bc
	gl_Position= mvpMatrix * vec4(gl_in[1].gl_Position.xyz + gn * 0.005, 1); EmitVertex();
	gl_Position= mvpMatrix * vec4(gl_in[2].gl_Position.xyz + gn * 0.005, 1); EmitVertex();
	EndPrimitive();
	
// arete ca
	gl_Position= mvpMatrix * vec4(gl_in[2].gl_Position.xyz + gn * 0.005, 1); EmitVertex();
	gl_Position= mvpMatrix * vec4(gl_in[0].gl_Position.xyz + gn * 0.005, 1); EmitVertex();
	EndPrimitive();
}
#endif

#ifdef FRAGMENT_SHADER
in vec3 geometry_color;

out vec4 fragment_color;
void main( )
{
	fragment_color= vec4(geometry_color, 1);
}

#endif
