#version 460 core
#extension GL_NV_gpu_shader5 : enable


#ifdef LIT
	uniform vec3 lightDir;
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
		vec3 pos;
		float pad1;
		vec3 dir;
		float pad2;
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
uniform float ambient = 0.2;

in vec3 crntPos;
in vec2 texCoords;
in vec3 vertNormal;
in vec3 normal;

out vec4 fragColor;


#ifdef LIT
// Helper: get diffuse + specular contribution from a directioned light
vec4 spotLight(LightObject lo){
	// early out for zero intensity or nearly black color
	if(lo.intensity <= 0.0 || length(lo.color.rgb) < 1e-6) { return vec4(0.0); }

	// ensure normals and directions are normalized
	vec3 N = normalize(normal);
	vec3 L = normalize(lo.dir); // direction from surface to light (caller should provide)
	
	// diffuse
	float diff = max(dot(N, L), 0.0);

	// specular (Blinn-Phong would be cheaper; keeping reflect-based original)
	float specularStrength = 0.50;
	vec3 V = normalize(camPos - crntPos);
	vec3 R = reflect(-L, N);
	float specFactor = pow(max(dot(V, R), 0.0), 16.0);
	float specular = specFactor * specularStrength;

	// angular falloff (spot cone)
	float angle = dot(vec3(0.0, -1.0, 0.0), -L); // this assumes spotlight points down - adjust per-light
	float spotAtt = clamp((angle - lo.radius_o) / (lo.radius_i - lo.radius_o), 0.0, 1.0);

	vec4 finalCol = lo.color * (diff * spotAtt * lo.intensity + ambient);

	if(hasSpecular){
		finalCol += vec4(texture(specular0, texCoords).r * specular * spotAtt, texture(specular0, texCoords).r * specular * spotAtt, texture(specular0, texCoords).r * specular * spotAtt, 0.0);
	}

	// ensure final color is at least ambient in rgb
	finalCol.rgb = max(finalCol.rgb, vec3(ambient));
	finalCol.a = 1.0;
	return finalCol;
}

vec4 pointLight(LightObject lo){
	if(lo.intensity <= 0.0 || length(lo.color.rgb) < 1e-6) { return vec4(0.0); }

	vec3 N = normalize(normal);
	vec3 L = normalize(lo.dir);

	// simple distance attenuation (tweak a/b as needed)
	float dist = length(lo.pos - crntPos);
	float a = 0.5;
	float b = 0.01;
	float att = 1.0 / (a * dist * dist + b * dist + 1.0);

	// diffuse
	float diff = max(dot(N, L), 0.0);

	// specular
	float specularStrength = 0.50;
	vec3 V = normalize(camPos - crntPos);
	vec3 R = reflect(-L, N);
	float specFactor = pow(max(dot(V, R), 0.0), 16.0);
	float specular = specFactor * specularStrength;

	vec4 finalCol = lo.color * (diff * att * lo.intensity + ambient);

	if(hasSpecular){
		float s = texture(specular0, texCoords).r * specular * att * lo.intensity;
		finalCol += vec4(s, s, s, 0.0);
	}

	finalCol.rgb = max(finalCol.rgb, vec3(ambient));
	finalCol.a = 1.0;
	return finalCol;
}

vec4 directionalLight(){
	vec3 N = normalize(normal);
	vec3 L = normalize(-lightDir); // direction from surface to light
	float diff = max(dot(N, L), ambient); // keep at least ambient

	// specular
	float specularStrength = 0.5;
	vec3 V = normalize(camPos - crntPos);
	vec3 R = reflect(-L, N);
	float specFactor = pow(max(dot(V, R), 0.0), 16.0);
	float specular = specFactor * specularStrength;

	vec3 rgb = (lightColor.rgb * diff);
	if(hasSpecular){
		float s = texture(specular0, texCoords).r * specular;
		rgb += vec3(s);
	}

	rgb = max(rgb, vec3(ambient));
	vec4 finalCol = vec4(rgb, 1.0);
	return finalCol;
}
#endif

#ifdef SHADOW // I need to somehow smoothe the lines
float ShadowPCF(vec3 projCoords){
    float shadow = 0.0;
    float bias = 0.0002;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);

    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            shadow += texture(ShadowMap, vec3(projCoords.xy + vec2(x,y) * texelSize, projCoords.z - bias));
        }
    }

    shadow /= 9.0;
    return shadow;
}
#endif

void main()
{
    // === Base color ===
    vec4 baseColor = hasDiffuse ? texture(diffuse0, texCoords) * color : color;

    // Initialize lighting and depth factor
    vec4 lightVal = vec4(1.0);
    float depthVal = 1.0;

    // === Optional depth-based fade (temporary separation) ===
    #ifdef DEPTH
        float dist = length(camPos - crntPos);
        depthVal = 1.0 - clamp(dist / 1000.0, 0.0, 1.0);
    #endif

    // === Lighting pass ===
    #ifdef LIT
        if (lightEnabled)
        {
            lightVal = directionalLight();

			#ifdef SHADOW
				vec3 projCoords = shadowFragPos.xyz / shadowFragPos.w;
				projCoords = projCoords * 0.5 + 0.5; // NDC -> [0,1]
				bool outside = any(lessThan(projCoords.xy, vec2(0.0))) || any(greaterThan(projCoords.xy, vec2(1.0))) || projCoords.z > 1.0 || projCoords.z < 0.0;
				float shadowFactor = 1.0;

				if(!outside){
					// epsilon to handle precision issues
					float eps = 0.001;

					// distance from map edge
					float edgeDist = min(min(projCoords.x, 1.0 - projCoords.x),
										 min(projCoords.y, 1.0 - projCoords.y));

					// factor 0.0 at edge, 1.0 inside
					float edgeFade = smoothstep(0.0, eps, edgeDist);

					// factor 0.0 if depth outside 0..1
					float depthFade = smoothstep(1.0 + eps, 1.0, projCoords.z);

					// normal PCF shadow sampling
					float shadowPCF = ShadowPCF(projCoords);

					// combine: near edge or outside = more shadow, inside = normal PCF
					shadowFactor = mix(ambient, shadowPCF, edgeFade * depthFade);
				}
				// apply to light
				lightVal.rgb *= max(shadowFactor, ambient);
			#endif

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
					default:
						break;
				}
			}
        }
        else
        {
            lightVal = vec4(vec3(ambient), 1.0);
        }
	#endif

    // === Final color composition ===
    fragColor = baseColor * lightVal * depthVal;

    // Opaque unless marked transparent
    fragColor.a = isTransparent ? fragColor.a : 1.0;
}