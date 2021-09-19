#version 430

//in vec4 varyingColor;
in vec2 tc;

out vec4 color;
flat in vec3 varyingNormal;
in vec3 varyingLightDir;
in vec3 varyingVertPos;


struct Light{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 position;
};

struct Material{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform Light light;
uniform Material material;
layout (binding=0) uniform sampler2D samp;

void main(void){
	
	// normalize
	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);

	// compute light reflection with respect to N
	vec3 R = normalize(reflect(-L,N));
	// get angle between the lidht and surface normalize
	float cosTheta = dot(L,N);
	float cosPhi = dot(V,R);

	// compute ADS contributions (per pixel), and combine to build output color
	vec3 ambient = ((globalAmbient * material.ambient) + (light.ambient * material.ambient)).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse.xyz * max(cosTheta,0.0);
	vec3 specular = light.specular.xyz * material.specular.xyz * pow(max(cosPhi,0.0),material.shininess);

	color = vec4 ((ambient + diffuse + specular),1.0) * texture(samp,tc);

}