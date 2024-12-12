#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 textureCoord;

uniform vec3 color;
uniform sampler2D ourTexture;

void main(){
        FragColor = texture(ourTexture, textureCoord);
}