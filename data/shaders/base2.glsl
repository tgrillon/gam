#version 330

#ifdef VERTEX_SHADER

layout(location = 0) in vec3 aPosition;
layout(location = 3) in vec4 aColor;

out vec3 vPosition;
out vec4 vColor;

uniform mat4 uMvMatrix;
uniform mat4 uMvpMatrix;

void main() {
	gl_Position = uMvpMatrix*vec4(aPosition, 1);

	vPosition = vec3(uMvMatrix*vec4(aPosition, 1));
	vColor = aColor; 
}
#endif

#ifdef FRAGMENT_SHADER

in vec3 vPosition;
in vec4 vColor;

out vec4 out_color;

uniform vec3 uLight;
uniform vec4 uLightColor = vec4(1, 1, 1, 1);
uniform vec4 uMeshColor = vec4(1, 1, 1, 1);

void main() {
	vec4 color = vColor;

	vec3 t = normalize(dFdx(vPosition));
	vec3 b = normalize(dFdy(vPosition));
	vec3 normal = normalize(cross(t, b));


	float cos_theta = max(0, dot(normal, normalize(uLight-vPosition)));
	color = color*uLightColor;

	// if(gl_FrontFacing==false) {
	// 	ivec2 pixel = ivec2(gl_FragCoord.xy/4)%ivec2(2, 2);
	// 	if((pixel.x^pixel.y)==0)
	// 		color = vec4(0.8, 0.4, 0, 1);
	// }

	color.rgb = color.rgb*cos_theta;

	out_color = color;
}
#endif
