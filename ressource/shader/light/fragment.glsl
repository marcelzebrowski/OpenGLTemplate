#version 330 core

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

in vec3 Normal;
in vec3 FragmentPosition;
//in vec3 LightPosition;

out vec4 FragColor;

vec3 calculateDiffuseColor(vec3 normal, vec3 lightDirection, vec3 diffuceColor){
    float diffuse = max(dot(normal,lightDirection),0.0f);
    return diffuse * diffuceColor;
}

vec3 calculateSpecularLight(vec3 viewDirection, vec3 reflectDirection, vec3 specularColor, float shininess){
    float specular = pow(max(dot(viewDirection, reflectDirection),0.0f),shininess);
    return  specularStrength * specular * specularColor;
}


void main(){

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    vec3 ambientLight = ambientStrength * lightColor;
    vec3 specularLight = calculateSpecularLight(viewDirection, reflectDirection, lightColor,32);        
    vec3 diffuseLight = calculateDiffuseColor(norm, lightDirection, lightColor);

    vec3 result = (ambientLight + diffuseLight + specularLight) * objectColor;
    FragColor = vec4(result, 1.0f); 
}