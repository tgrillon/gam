
#version 330

#ifdef VERTEX_SHADER
layout(location= 0) in vec3 position;
uniform mat4 mvpMatrix;

void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
}
#endif

#ifdef FRAGMENT_SHADER
out vec4 fragment_color;

void main( )
{
    if(gl_FrontFacing == false)
        fragment_color= vec4(0.4, 0.2, 0, 1);
    else
        fragment_color= vec4(0.8, 0.4, 0, 1);
}
#endif
