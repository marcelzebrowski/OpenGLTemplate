#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
uniform sampler2D texture;

void main(){
    vec4 texColor = texture(texture,textureCoord);
    FragColor = vec4(texColor.rgb, texColor.a * 1.0);
}