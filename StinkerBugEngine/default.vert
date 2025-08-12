#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;


uniform mat4 model;
uniform mat4 camMatrix; // proj * view

out vec4 vertColor;

void main(){
	vec3 crntPos = vec3(model * vec4(aPos, 1.0));


	vertColor = vec4(aNormal, 1.0);

	gl_Position = camMatrix * vec4(crntPos, 1.0);
}