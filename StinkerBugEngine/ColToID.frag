#version 460 core


uniform vec4 color;
in vec3 crntPos;

out vec4 fragColor;
void main(){
	fragColor = color;
}