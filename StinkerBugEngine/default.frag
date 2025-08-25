#version 460 core


#ifdef LIT
	uniform vec3 lightDir;
	uniform vec4 lightColor;
#endif

vec4 shadowColor = vec4(0.05, 0.1, 0.2, 1.0);

uniform sampler2DShadow ShadowMap;


uniform vec3 camPos;
uniform vec4 color;
uniform float ambient = 0.2;

in vec3 crntPos;
in vec3 normal;
in vec4 vertColor;

out vec4 fragColor;
in vec4 shadowFragPos;

#ifdef LIT
vec4 directionalLight(){
	
	// easy to understand
	vec3 lightDirection = -lightDir;
	
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

float ShadowPCF(vec3 projCoords)
{
    float shadow = 0.0;
    float bias = 0.0001;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);

    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            shadow += texture(ShadowMap, vec3(projCoords.xy + vec2(x,y) * texelSize,
                                             projCoords.z - bias));
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main(){
	vec4 lightVal = vec4(1.0);
	float depthVal = 1.0;
	float shadowVal = 1.0;

	#ifdef DEPTH
		depthVal = 1.0 - (min(length(camPos - crntPos), 1000.0) / 1000.0); // Seperates object a bit, temporaray
	#endif

	#ifdef LIT	
		lightVal = directionalLight();
	#endif

	vec3 projCoords = shadowFragPos.xyz / shadowFragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	shadowVal = ShadowPCF(projCoords); 


	fragColor = color * lightVal * (shadowVal + ((1-shadowVal) * shadowColor)) * depthVal;

}