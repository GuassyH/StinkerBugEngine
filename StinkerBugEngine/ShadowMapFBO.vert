#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 light_WVP;

void main(){
	gl_Position = light_WVP * vec4(aPos, 1.0f);
}