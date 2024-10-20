#version 330

#ifdef VERTEX_SHADER

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexcoord;
layout(location = 2) in vec3 aNormal;

out vec3 vPosition;
out vec4 vColor;
out vec3 vNormal;
out vec2 vTexcoord;

uniform mat4 uMvMatrix;
uniform mat4 uMvpMatrix;
uniform mat4 uNormalMatrix;
uniform float uPointSize;

void main() {
	gl_Position = uMvpMatrix*vec4(aPosition, 1);
	gl_PointSize = uPointSize;

	vTexcoord = aTexcoord;
	vNormal = mat3(uNormalMatrix)*aNormal;
	vColor = vec4(normalize(mat3(uNormalMatrix)*aNormal), 1.0);

	vPosition = vec3(uMvMatrix*vec4(aPosition, 1));
}
#endif

#ifdef FRAGMENT_SHADER

in vec2 vTexcoord;

in vec3 vNormal;
in vec4 vColor;

in vec3 vPosition;

out vec4 out_color;

uniform vec3 uLight;
uniform vec4 uLightColor = vec4(1, 1, 1, 1);
uniform vec4 uMeshColor = vec4(1, 1, 1, 1);
uniform bool uShowCurvature;
uniform bool uShowNormalColor;
uniform bool uShowSmoothNormal;
uniform bool uShowHeatDiffusion;
uniform sampler2D uHeatDiffusionTex;

void main() {
	vec4 color = uMeshColor;

	if(uShowCurvature) {
		float s = 1;
		vec3 curv = vec3(vTexcoord.x*s*2, vTexcoord.x*s, vTexcoord.x*s*4);
		color = vec4(curv, 1.0);
	} else if(uShowHeatDiffusion) {
		color = texture(uHeatDiffusionTex, vec2(vTexcoord.x/100, 0.0));
	}

	vec3 t = normalize(dFdx(vPosition));
	vec3 b = normalize(dFdy(vPosition));
	vec3 normal = normalize(cross(t, b));

	if(uShowNormalColor) {
		color = vColor;
	}

	if(uShowSmoothNormal) {
		normal = normalize(vNormal);
	}

	float cos_theta = max(0, dot(normal, normalize(uLight-vPosition)));
	color = color*uLightColor;

	if(gl_FrontFacing==false) {
		ivec2 pixel = ivec2(gl_FragCoord.xy/4)%ivec2(2, 2);
		if((pixel.x^pixel.y)==0)
			color = vec4(0.8, 0.4, 0, 1);
	}

	color.rgb = color.rgb*cos_theta;

	out_color = color;
}
#endif
