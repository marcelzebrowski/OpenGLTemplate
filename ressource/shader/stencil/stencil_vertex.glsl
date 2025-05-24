layout (location = 0) in vec3 aPos;
uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    vec3 scaledPos = aPos* scale;
    gl_Position = projection * view * model * vec4(scaledPos, 1.0);
}