#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec4 color;

vec3 calculateDiffuseColor(vec3 normal, vec3 lightDirection, vec3 diffuceColor){
    float diffuse = max(dot(normal,lightDirection),0.0f);
    return diffuse * diffuceColor;
}

vec3 calculateSpecularLight(vec3 viewDirection, vec3 reflectDirection, vec3 specularColor, float shininess){
    float specular = pow(max(dot(viewDirection, reflectDirection),0.0f),shininess);
    return  specularStrength * specular * specularColor;
}

vec4 applyTransformation(vec3 pos, mat4 model, mat4 view, mat4 projection){
    return projection * view * model * vec4(pos,1.0f);
}

void main(){

    vec3 FragmentPosition = vec3(model*vec4(aPos,1.0f)); // berechne Weltkoordinate
    // Normal=mat3(transpose(inverse(model)))*aNormal
    vec3 Normal = mat3(transpose(inverse(model))) * aNormal; // normale transformieren

    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(lightPosition - FragmentPosition);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);
    vec3 reflectDirection = reflect(-lightDirection, norm);

    vec3 ambientLight = ambientStrength * lightColor;
    vec3 specularLight = calculateSpecularLight(viewDirection, reflectDirection, lightColor,32);        
    vec3 diffuseLight = calculateDiffuseColor(norm, lightDirection, lightColor);

    vec3 result = (ambientLight + diffuseLight + specularLight) * objectColor;
    gl_Position =  applyTransformation(aPos, model, view, projection);
    color = vec4(result,1.0);
}