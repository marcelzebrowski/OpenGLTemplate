#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 texCoord;

out vec2 tc;
//out vec4 varyingColor;
flat out vec3 varyingNormal; // eye space vertex normal
out vec3 varyingLightDir; // vector pointing to the light
out vec3 varyingVertPos; // vertex position in eye space


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

// light & material
uniform vec4 globalAmbient;
uniform Light light;
uniform Material material;

// matrix 
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;


layout (binding=0) uniform sampler2D samp;

void main(void){
	tc = texCoord;
	varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz;
	varyingLightDir = light.position - vertPos;
	varyingNormal = (norm_matrix * vec4(vertNormal, 1.0)).xyz;
	gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}