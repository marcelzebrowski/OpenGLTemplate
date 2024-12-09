#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;


uniform float time;

out vec3 vertexColor;

void main(){

   // Bewegung entlang der x-Achse
    float translation = mod(time * 0.5, 2.0) - 1.0; // Bewegung von -1 bis +1
    float wave = sin((aPos.x + translation) * 3.14159); // Sinus-Wellenform

    // Position des Vertex setzen
    gl_Position = vec4(aPos.x + translation, aPos.y + wave * 0.1, aPos.z, 1.0); // Amplitude: 0.1
}