#version 460 core


uniform vec4 color;
in vec3 crntPos;

uniform bool hovered;

out vec4 fragColor;
void main(){
	fragColor = hovered ? vec4(1.0, 1.0, 0.6, 1.0) : color;
}