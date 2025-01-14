#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPosition;

in vec3 Normal;
in vec3 FragmentPosition;

out vec4 FragColor;

vec3 calculateDiffuseColor(vec3 normal, vec3 lightDirection){
    float diffuse = max(dot(normal,lightDirection),0.0f);
    return light.diffuse * (diffuse * material.diffuse);
}

vec3 calculateSpecularLight(vec3 viewDirection, vec3 reflectDirection){
    float spec = pow(max(dot(viewDirection, reflectDirection),0.0f),material.shininess);
    return light.specular * (spec * material.specular);
}

vec3 calculateAmbientLight(){
    return light.ambient * material.ambient;
}

void main(){

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragmentPosition);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    vec3 ambientLight = calculateAmbientLight();
    vec3 diffuseLight = calculateDiffuseColor(norm, lightDirection);
    vec3 specularLight = calculateSpecularLight(viewDirection, reflectDirection);       

    vec3 result = ambientLight + diffuseLight + specularLight;
    FragColor = vec4(result, 1.0f); 
}