layout (location = 0) in vec3 aPos;
uniform float scale;

void main(){
    vec3 scaledPos = aPos * scale;
    gl_Position = vec4(scaledPos, 1.0);
}