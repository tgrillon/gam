
#version 330

#ifdef VERTEX_SHADER
uniform vec2 viewport;
uniform sampler2D zbuffer;
uniform sampler2D color;
uniform sampler2D position;
uniform sampler2D normal;
uniform sampler2D data;

uniform int mode;

uniform mat4 mvpMatrix;

out vec4 vertex_color;

void main( )
{
    // retrouve les coordonnees du pixel
    ivec2 pixel= ivec2(gl_VertexID % int(viewport.x), gl_VertexID / int(viewport.x));
    
    // recupere les infos
    vertex_color= vec4(0);
    if(mode == 0)
        vertex_color= abs(texelFetch(position, pixel, 0));
    else if(mode == 1)
        vertex_color= abs(texelFetch(color, pixel, 0));
    else if(mode == 2)
        vertex_color= abs(texelFetch(normal, pixel, 0));
    else if(mode == 3)
        vertex_color= abs(texelFetch(data, pixel, 0));
    
    // reprojette le fragment pour le point de vue debug, si necessaire
    float z= texelFetch(zbuffer, pixel, 0).r;
    if(z == 1)
        gl_Position= vec4(1/0);         // discard, force les coordonnees a nan
    else
        gl_Position= mvpMatrix * vec4(pixel.xy + vec2(0.5), z, 1);
}
#endif

#ifdef FRAGMENT_SHADER
in vec4 vertex_color;

out vec4 fragment_color;
void main( )
{
    fragment_color= vertex_color;
}
#endif
