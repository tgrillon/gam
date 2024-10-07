#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 aPosition;

uniform mat4 uMvpMatrix;

void main()
{   
    gl_Position= uMvpMatrix * vec4(aPosition, 1.0); 
}
#endif

#ifdef GEOMETRY_SHADER
layout(triangles) in; 
layout(line_strip, max_vertices= 4) out; 

void main()
{
    gl_Position= gl_in[0].gl_Position; 
    EmitVertex();
    gl_Position= gl_in[1].gl_Position; 
    EmitVertex();
    gl_Position= gl_in[2].gl_Position; 
    EmitVertex();
    gl_Position= gl_in[0].gl_Position; 
    EmitVertex();
}
#endif


#ifdef FRAGMENT_SHADER
out vec4 out_color; 

uniform vec4 uEdgeColor=vec4(0., 0., 0., 1.); 

void main()
{
    out_color= uEdgeColor;
}
#endif
