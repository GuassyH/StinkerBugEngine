#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 modelMatrix;
uniform mat4 camMatrix; // proj * view
uniform mat4 rotationMatrix;
uniform mat4 light_WVP;

out vec2 texCoords;
out vec4 shadowFragPos;
out vec4 vertColor;
out vec3 normal;
out vec3 crntPos;
void main(){
	crntPos = vec3(modelMatrix * vec4(aPos, 1.0));

	normal = normalize(vec3(rotationMatrix * vec4(aNormal, 1.0)));
	vertColor = vec4(vec3(0.8), 1.0);

	gl_Position = camMatrix * vec4(crntPos, 1.0);
	shadowFragPos = light_WVP * vec4(crntPos, 1.0);
	texCoords = aTex;
}