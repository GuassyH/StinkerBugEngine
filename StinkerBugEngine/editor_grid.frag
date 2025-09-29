#version 460 core


in vec2 texCoords;
in vec3 crntPos;
uniform vec3 camPos;

out vec4 fragColor;
void main(){
	fragColor = vec4(1.0);
	float dist = distance(vec3(camPos.x, 0, camPos.z), vec3(crntPos.x, 0, crntPos.z)) / 50.0;

	if (dist > 1.0) { discard; }

	float x_pos = ((texCoords.x*100.0) + camPos.x) * 2.0;
	float z_pos = ((texCoords.y*100.0) + camPos.z) * 2.0;

	float gx = mod(round(x_pos * 50.0) / 50.0, 2.0);
	float gz = mod(round(z_pos * 50.0) / 50.0, 2.0);

	// SHOULD ANTI ALIAS (but i dont know how)
	bool is_grid_line = (abs(gx) < 0.01) || (abs(gz) < 0.01);

	if(is_grid_line){
		fragColor = vec4(0.5) * (1 - dist);
	}
	else{
		discard;
	}
}