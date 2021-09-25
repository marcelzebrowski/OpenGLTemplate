#version 430
layout (location = 0) in vec3 vertPos;

uniform float u_time; // Zeit
uniform vec2 u_k; // scale Factor
out vec3 varyingVertPos; // Vertex Position

void main(void){
    varyingVertPos = vertPos;
    gl_Position = vec4(vertPos,1.0);
}