#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
uniform sampler2D texture;

void main(){
    FragColor = texture(texture,textureCoord);
    //FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}