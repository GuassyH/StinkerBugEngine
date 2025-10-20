#version 460 core
#extension GL_NV_gpu_shader5 : enable


#ifdef LIT
	in vec3 lightDir;
	uniform vec4 lightColor;

	// simple integer light type constants (GLSL enum-style portability)
	const int LIGHT_DIRECTIONAL = 0;
	const int LIGHT_SPOTLIGHT   = 1;
	const int LIGHT_POINT       = 2;
	const int LIGHT_AREA        = 3;

	struct LightObject {
		int type;
		int pad0;
		float radius_i;	
		float radius_o;	
		float radius;
		float intensity;
		float angle;
		int pad2;
		vec3 pos;
		float pad3;
		vec3 dir;
		float pad4;
		vec4 color;
	};

	layout (std430, binding = 0) buffer LightObjectsBuffer {
		LightObject lightObjs[];
	};

	uniform int numLights;
#endif

#ifdef SHADOW
	uniform sampler2DShadow ShadowMap;
	in vec4 shadowFragPos;
#endif

vec4 shadowColor = vec4(0.9, 0.9, 0.95, 1.0);

uniform sampler2D diffuse0;
uniform sampler2D specular0;

uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool isTransparent;

uniform bool lightEnabled;

uniform vec3 camPos;
uniform vec4 color;
uniform float ambient;


in vec3 crntPos;
in vec2 texCoords;
in vec3 vertNormal;
in vec3 normal;

out vec4 fragColor;



#ifdef LIT

vec4 spotLight(LightObject lo){
	vec3 finalCol = vec3(1.0);
    
	vec3 lightVec = lo.pos - crntPos;
	float dist = length(lightVec);
	if(dist >= lo.radius) { return vec4(0.0); }
	
	float dst_intensity = 1.0 - (dist / lo.radius);

	// Diffuse
	vec3 lightDir = normalize(lo.pos - crntPos);   // frag > light (for diffuse)
	vec3 toFrag   = -lightDir;                     // light > frag (for cone)
	float diffuse = max(dot(normal, lightDir), 0.0);

	// Cone Stuff
	float cutOff = cos(radians(lo.angle));
	float theta = dot(lo.dir, toFrag);
	
	float outerCutOff = cos(radians(lo.angle + 5.0));
	float fo_intensity = clamp((theta - outerCutOff) / (cutOff - outerCutOff), 0.0, 1.0);

	// specular lighting
	float specularLight = 0.50;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-toFrag, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
	float specular = specAmount * specularLight;

	// If inside the cone
	if (theta > outerCutOff) {
		finalCol = lo.color.rgb * dst_intensity * diffuse * fo_intensity * lo.intensity;

		if(hasSpecular){
			float s = texture(specular0, texCoords).r * specular * fo_intensity;
			finalCol += vec3(s);
		}
		return vec4(finalCol, 1.0);
	}
	
	return vec4(0.0);
}

vec4 pointLight(LightObject lo) {
	

	// use inverse square law to calculate intensity
	vec3 lightVec = lo.pos - crntPos;
	float dist = length(lightVec);


	if(dist >= lo.radius_o) { return vec4(0.0); }
	
	float dst_intensity = 0.0;

	if(dist < lo.radius_i){
		dst_intensity = 1.0;
	}
	else{
		float range = lo.radius_o - lo.radius_i;
		float dist_scaled = (dist-lo.radius_i) / (range);
		dst_intensity = 1.0 - dist_scaled;
		dst_intensity = max(dst_intensity, 0.0);
	}
	

	
	// diffuse lighting
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0);

	// specular lighting
	float specularLight = 0.50;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0), 16);
	float specular = specAmount * specularLight;

	float cl_intensity = max(lo.intensity, 0.0);

    vec3 finalCol = lo.color.rgb * (diffuse * dst_intensity) * cl_intensity;

    if(hasSpecular) {
        float s = texture(specular0, texCoords).r * specular * cl_intensity;
        finalCol += vec3(s);
    }

    
	return vec4(finalCol, 0.0);
}

vec4 directionalLight(){
	vec3 N = normalize(normal);
	vec3 L = normalize(-lightDir); // direction from surface to light

	float NdotL = max(dot(N,L), ambient);

	// specular
	float specularStrength = 0.5;
	vec3 V = normalize(camPos - crntPos);
	vec3 R = reflect(-L, N);
	float specFactor = pow(max(dot(V, R), 0.0), 16.0);
	float specular = specFactor * specularStrength;

	vec3 rgb = lightColor.rgb * NdotL;

	if(hasSpecular){
		float s = texture(specular0, texCoords).r * specular;
		rgb += vec3(s);
	}

	vec4 finalCol = vec4(rgb, 1.0);
	return finalCol;
}
#endif

#ifdef SHADOW // I need to somehow smoothe the lines

float ShadowPCF(vec3 projCoords){
    vec3 result = vec3(0.0);
	float shadow = 0.0;
    float bias = 0.0002;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
	
	int samples = 3;
	for(int x=-1; x<=1; ++x){
		for(int y=-1; y<=1; ++y){
			vec2 offset = vec2(x, y) * texelSize;
			shadow += texture(ShadowMap, vec3(projCoords.xy + offset, projCoords.z - bias));
		}
	}
	shadow /= float(samples * samples);

    return mix(ambient, 1.0, shadow);
}

#endif

void main()
{
    // === Base color ===
    vec4 baseColor = hasDiffuse ? texture(diffuse0, texCoords) * color : color;

    // Initialize lighting and depth factor
    vec4 lightVal = vec4(1.0);

    // === Lighting pass ===
    #ifdef LIT
        if (lightEnabled)
        {
            lightVal = directionalLight();

			#ifdef SHADOW
				if(dot(normal, lightDir) < -0.1) { 
					vec3 projCoords = shadowFragPos.xyz / shadowFragPos.w;
					projCoords = projCoords * 0.5 + 0.5; // NDC -> [0,1]
					bool outside = any(lessThan(projCoords.xy, vec2(0.0))) || any(greaterThan(projCoords.xy, vec2(1.0))) || projCoords.z > 1.0 || projCoords.z < 0.0;
					float shadowFactor = 1.0;

					shadowFactor = outside ? shadowFactor : ShadowPCF(projCoords);

					// apply to light
					lightVal.rgb *= shadowFactor;
				}
			#endif
        }
        else{
            lightVal = vec4(vec3(ambient), 1.0);
        }

		for (int i = 0; i < numLights; i++) {
			LightObject lo = lightObjs[i];
			// Calculate Light;
			switch(lo.type){
				case LIGHT_SPOTLIGHT:
					lightVal += spotLight(lo);
					break;
				case LIGHT_POINT:
					lightVal += pointLight(lo);
					break;
				case LIGHT_AREA:
					break;
				default:
					break;
			}
		}

	#endif

    // === Final color composition ===
    fragColor = baseColor * lightVal;
	// fragColor.rgb += ambient;

    // Opaque unless marked transparent
    fragColor.a = isTransparent ? fragColor.a : 1.0;
}