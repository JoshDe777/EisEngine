#version 460 core

#define PI 3.1415

// This fragment shader is a different illumination model basing upon fresnel light approximations.
// Any shader using this FS MUST use vert-shader3D.vert 
//      or an equivalent vertex shader outputting complex geometric assets.
// Lighting is calculated from PointLight structs established from the 
//      Barnes-Hut light cuts in the Lighting System, which are accessed via a 
//      Shader Storage Buffer Object (SSBO).
// Material properties required include:
//      - diffuse colour (object's base colour irrespective of texture.)
//      - texture tiling (directly used for texture repetition)
//      - alpha [transparency] (directly applied to the entire object)
//      - roughness [r] (used to establish shininess as 1 - r in [0, 1] range.)
// Supported textures include:
//      diffuse/albedo, normal/bump map (TODO), specular map (TODO)

struct PointLight{
    vec3 pos;               // light centre in world space for illumination direction & attenuation.
    float _padding;         // required because of memory reading method according to Claude... (vec3 expected to start 16bit sequence so vec3 usage could read into the next struct)
    vec3 emission;          // light emission colour.
    float I;                // aggregated illumination strength of the light cluster.
};

// VS inputs
in vec3 fragPos;            // fragment position in world space for lighting vectors.
in vec2 TexCoords;          // fragment texture coords in texture space.
in vec3 fragNormal;         // fragment normal in model space.
in vec3 fragTan;            // fragment tangent in model space.
in vec3 fragBitan;          // fragment bitangent in model space.

// material properties
uniform vec3 diffuse;       // object base colour [RGB].
uniform float alpha;        // object transparency
uniform float tiling;       // tiling factor for texture repetition.
uniform float roughness;    // object roughness; inversely proportional to shininess.
uniform float F0;           // Fresnel factor F0 influencing the sharpness of the BRDF's specular lobe.
uniform sampler2D diffMap;  // diffuse map/texture applied to the geometry (alters colour).
uniform sampler2D nMap;     // normal map/texture applied to the geometry (alters light effect directions).

// world properties
uniform float ambient;      // ambient lighting level. applied as base illumination before complex calculation.


// Lighting-related Input
layout(std430, binding=0) readonly buffer LightBuffer {
    PointLight[] lights;    // array/list of light clusters effecting on the object.
};
uniform int nLights;        // amount of effecting clusters. Will render in ambient only if set to 0.
uniform vec3 camPos;        // camera position in world space for view vectors (specularity).

// output(s)
out vec4 fragColor;

vec3 getNormalInWorldSpace(){
    // bump val to [-1, 1] for negative displacement (craters)
    vec3 mapNormal = texture(nMap, TexCoords).xyz;
    mapNormal = normalize(2 * (mapNormal - vec3(0.5)));

    vec3 normal = normalize(fragNormal);
    vec3 tan = normalize(fragTan);
    vec3 bitan = normalize(fragBitan);

    mat3 tanSpaceMat = mat3(tan, bitan, normal);

    return normalize(tanSpaceMat * mapNormal);
}

vec3 calculateFragColor(vec4 base){
    vec3 result = ambient * base.xyz;
    // calculate normal accounting for nMap
    vec3 normal = getNormalInWorldSpace();
    // apply diffuse and specular changes for each light affecting the object.
    for(int i = 0; i < nLights; i++){
        PointLight light = lights[i];

    // diffuse:
        vec3 lightDir = normalize(light.pos - fragPos);
        float brightness = max(0, dot(normal, lightDir));
        // calculate distance between light src and obj.
        float dist = max(distance(light.pos, fragPos), 0.01);
        float attenuation = light.I / pow(dist, 3);
        result += light.emission * base.xyz * brightness * attenuation;

    // specular:
        vec3 view = normalize(camPos - fragPos);
        vec3 vHalf = normalize(lightDir + view);

        // fresnel reflectance approximation
        float m = max(1e-4, dot(lightDir, vHalf));
        float F = pow((1 - m), 5) * (1 - F0) + F0;

        // beckmann facet vis - surface scattering approximation
        float angle = max(0.02, dot(normal, vHalf));
        float clamped_roughness = max(0.02, roughness);
        float r1 = 1 / (4 * clamped_roughness * clamped_roughness * pow(angle, 4));
        float r2 = (angle * angle - 1) / (clamped_roughness * clamped_roughness * angle * angle);
        float D = r1 * exp(r2);

        // Geometric shadowing (specular attenuation)
        float g = max(0, dot(normal, view));
        float g1 = (2.0 * angle * g) / m;
        float g2 = (2 * angle * brightness) / m;
        float G = min(1, min(g1, g2));

        float denumerator = PI * brightness * g;
        if(denumerator == 0)
            denumerator = 0.001;
        float ks = (F * D * G) / denumerator;

        result += brightness * light.emission * ks * attenuation;
    }
    return result;
}

void main() {
    fragColor = vec4(calculateFragColor(vec4(diffuse, 1.0)).xyz, alpha) * texture(diffMap, TexCoords * tiling);
}
