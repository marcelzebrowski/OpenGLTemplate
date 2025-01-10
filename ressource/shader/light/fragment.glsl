#version 330 core

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

// from vertex shader
in vec3 Normal;
in vec3 FragmentPosition;



out vec4 FragColor;

void main(){

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);

    // Diffuse Light
    float diffuse = max(dot(norm,lightDirection),0.0f);

    // Specular Light
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float specular = pow(max(dot(viewDirection, reflectDirection),0.0f),32);    

    vec3 ambientLight = ambientStrength * lightColor;
    vec3 diffuseLight = diffuse * lightColor;
    vec3 specularLight = specularStrength * specular * lightColor;    

    vec3 result = (ambientLight + diffuseLight + specularLight) * objectColor;
    FragColor = vec4(result, 1.0f); 
}