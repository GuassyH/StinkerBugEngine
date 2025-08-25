#version 460 core

uniform int screenWidth;
uniform int screenHeight;
uniform float FOVdeg;

uniform vec3 sunDir;
uniform vec4 sunColor;

uniform vec3 camForward;
uniform vec3 camRight;
uniform vec3 camUp;
uniform float camNearPlane;
uniform float camFarPlane;
uniform vec3 camPos;

vec3 SkyBoxLightColor = vec3 (0.5, 0.7, 0.9);
vec3 SkyBoxDarkColor = vec3 (0.3, 0.4, 0.6);

in vec3 vertColor;
out vec4 fragColor;
void main(){
    // Get the ray dir and so on
    vec2 pixelCoords = gl_FragCoord.xy;

    float x = -(float(pixelCoords.x * 2 - screenWidth) / screenWidth); // transforms to [-1.0, 1.0]
	float y = -(float(pixelCoords.y * 2 - screenHeight) / screenHeight); // transforms to [-1.0, 1.0]

	vec2 rayCoord = vec2(x, y);

	float fov = radians(FOVdeg); // adjust as needed
	float aspect = float(screenWidth) / float(screenHeight);
	float scale = tan(fov * 0.5);

	vec3 rayDir = normalize(	(camForward) + rayCoord.x * aspect * scale * -camRight + rayCoord.y * scale * -camUp	);
	vec3 rayOrigin = camPos;

	vec3 skyColor = mix(SkyBoxLightColor, SkyBoxDarkColor, pow(1.0 - rayDir.y, 2.0));

	vec4 bloom = vec4(0.0);
	if(dot(rayDir, -sunDir) > 0.999){ bloom = pow(max(0, dot(rayDir, -sunDir)), 4000) * sunColor; }
	// if(dot(rayDir, -sunDir) > 0.999){ fragColor = sunColor; return; }

	fragColor = vec4(skyColor, 1.0) + bloom;
}