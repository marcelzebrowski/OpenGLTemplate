#version 330 core
out vec4 FragColor;

in vec2 textureCoord;
uniform sampler2D texture;
uniform float alpha;
uniform float time;

void main(){

    float strength = 0.03;
    float frequence = 8.0f;

    vec2 wobbleCord = textureCoord;
    wobbleCord.x += sin(textureCoord.x * frequence + time * 1.5) * strength;
    wobbleCord.y += cos(textureCoord.y * frequence + time * 1.5) * strength;

    vec4 texColor = texture(texture,wobbleCord);
    FragColor = vec4(texColor.rgb, texColor.a * alpha);
}