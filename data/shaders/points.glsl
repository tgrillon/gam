#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 aPosition;

uniform mat4 uMvpMatrix;
uniform float uPointSize;

void main()
{   
    gl_Position= uMvpMatrix * vec4(aPosition, 1.0); 
    gl_PointSize= uPointSize;
}
#endif

#ifdef FRAGMENT_SHADER
out vec4 out_color; 

uniform vec4 uPointColor=vec4(0., 0., 0., 1.); 

void main()
{
    out_color= uPointColor;
}
#endif
