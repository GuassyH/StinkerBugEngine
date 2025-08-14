#version 460 core


#ifdef LIT
	vec3 lightDir = vec3(10, 13, 8.4);
	vec4 lightColor = vec4(1);
#endif

#ifdef SHADOW
	vec4 shadowColor = vec4(0.2, 0.3, 0.8, 1.0)
#endif

uniform vec3 camPos;
uniform vec4 color;

in vec3 crntPos;
in vec3 normal;
in vec4 vertColor;

out vec4 fragColor;

float ambient = 0.2;

#ifdef LIT
vec4 directionalLight(){
	
	// easy to understand
	vec3 lightDirection = normalize(lightDir);
	
	// diffuse lighting
	lightDirection = normalize(lightDirection);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// specular lighting
	float specularLight = 0.5;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16.0);
	float specular = specAmount * specularLight;

	// return texture(diffuse0, texCoord) * vec4(vertColor, 1.0f) * lightColor * (diffuse + ambient) + (texture(specular0, texCoord).r * specular);
	vec4 finalCol = (lightColor * (diffuse + ambient)) + specular;
	finalCol.a = 1.0;
	return finalCol;
};
#endif

void main(){
	vec4 lightVal = vec4(1.0);
	float depthVal = 1.0;
	float shadowVal = 1.0;

	#ifdef DEPTH
		depthVal = 1.0 - (min(length(camPos - crntPos), 100.0) / 100.0); // Seperates object a bit, temporaray
	#endif

	#ifdef LIT	
		lightVal = directionalLight();
		#ifdef SHADOW
			// Calculate Shadow 
		#endif
	#endif

	fragColor = color * lightVal * depthVal;
}