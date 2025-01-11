#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPosition;

out vec3 Normal;
out vec3 FragmentPosition;
//out vec3 LightPosition;

vec4 applyTransformation(vec3 pos, mat4 model, mat4 view, mat4 projection){
    return projection * view * model * vec4(pos,1.0f);
}

void main(){
    FragmentPosition = vec3(model*vec4(aPos,1.0f)); // berechne Weltkoordinate
    // Normal=mat3(transpose(inverse(model)))*aNormal
    Normal = mat3(transpose(inverse(model))) * aNormal; // normale transformieren
    gl_Position =  applyTransformation(aPos, model, view, projection);
    //LightPosition = vec3(view * vec4(lightPosition,1.0f));
}