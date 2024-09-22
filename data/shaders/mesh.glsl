
#version 330

#ifdef VERTEX_SHADER

layout(location= 0) in vec3 position;
uniform mat4 mvpMatrix;


#ifdef USE_TEXCOORD
    layout(location= 1) in vec2 texcoord;
    out vec2 vertex_texcoord;
#endif

#ifdef USE_NORMAL
    layout(location= 2) in vec3 normal;
    uniform mat4 normalMatrix;
    out vec3 vertex_normal;
#endif

#if defined USE_LIGHT || !defined USE_NORMAL
    uniform mat4 mvMatrix;
    out vec3 vertex_position;
#endif

#ifdef USE_COLOR
    layout(location= 3) in vec4 color;
    out vec4 vertex_color;
#endif


void main( )
{
    gl_Position= mvpMatrix * vec4(position, 1);
    
#ifdef USE_TEXCOORD
    vertex_texcoord= texcoord;
#endif

#ifdef USE_NORMAL
    vertex_normal= mat3(normalMatrix) * normal;
#endif

#if defined USE_LIGHT || !defined USE_NORMAL
    vertex_position= vec3(mvMatrix * vec4(position, 1));
#endif

#ifdef USE_COLOR
    vertex_color= color;
#endif
}
#endif


#ifdef FRAGMENT_SHADER

#ifdef USE_TEXCOORD
    in vec2 vertex_texcoord;
    uniform sampler2D diffuse_color;
#endif

#ifdef USE_ALPHATEST
    uniform float alpha_min= 0.3;
#endif


#ifdef USE_NORMAL
    in vec3 vertex_normal;
#endif

#ifdef USE_COLOR
    in vec4 vertex_color;
#endif

#ifdef USE_LIGHT
uniform vec3 light;
uniform vec4 light_color;
#endif

#if defined USE_LIGHT || !defined USE_NORMAL
in vec3 vertex_position;
#endif

uniform vec4 mesh_color= vec4(1, 1, 1, 1);

out vec4 fragment_color;

void main( )
{
    vec4 color= mesh_color;
#ifdef USE_COLOR
    color= vertex_color;
#endif

#ifdef USE_TEXCOORD
    color= color * texture(diffuse_color, vertex_texcoord);
    
    #ifdef USE_ALPHATEST
        if(color.a < alpha_min)
            discard;
    #endif
#endif

    vec3 normal= vec3(0, 0, 1);
#ifdef USE_NORMAL
    normal= normalize(vertex_normal);
#else
    vec3 t= normalize(dFdx(vertex_position));
    vec3 b= normalize(dFdy(vertex_position));
    normal= normalize(cross(t, b));
#endif

#ifndef USE_ALPHATEST
    float cos_theta= 1;
    #ifdef USE_LIGHT
        cos_theta= max(0, dot(normal, normalize(light - vertex_position)));         // eclairage, uniquement des faces bien orientees
        color= color * light_color;
    #else
        cos_theta= normal.z; //abs(dot(normal, normalize(vertex_position)));
        //~ cos_theta= abs(dot(normal, normalize(vertex_position)));
    #endif
    
    // hachure les triangles mal orientes
    if(gl_FrontFacing == false)
    {
        ivec2 pixel= ivec2(gl_FragCoord.xy / 4) % ivec2(2, 2);
        if((pixel.x ^ pixel.y) == 0)
            color= vec4(0.8, 0.4, 0, 1);
    }
    
    color.rgb= color.rgb * cos_theta;
#endif
    
    fragment_color= color;
}
#endif
