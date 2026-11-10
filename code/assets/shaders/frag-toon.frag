#version 460 core

// This Fragment Shader is a non-photorealistic renderer that colours objects in 
//      steps of lighting, based on their illumination on a given fragment.
// Any shader using this FS MUST use vert-shader3D.vert 
//      or an equivalent vertex shader outputting complex geometric assets.
// Lighting is calculated from PointLight structs established from the 
//      Barnes-Hut light cuts in the Lighting System, which are accessed via a 
//      Shader Storage Buffer Object (SSBO).
// Required Material properties include:
//      - texture tiling (directly used for texture repetition)
//      - alpha [transparency] (directly applied to the entire object)
//      - metallic
//      - roughness [r] (used to establish shininess as 1 - r in [0, 1] range.)

#define MAX_LIGHTS 1

struct PointLight{
    vec3 pos;
    float _padding;
    vec3 emission;
    float I;
};

// VS inputs
in vec3 fragPos;
in vec2 TexCoords;
in vec3 fragNormal;
in vec3 fragTan;
in vec3 fragBitan;

// Vec3s
uniform vec3 diffuse;
uniform vec3 camPos;

// Samplers
uniform sampler2D image;
uniform sampler2D nMap;

// floats
uniform float tiling;
uniform float alpha;
uniform float metallic;
uniform float roughness;
uniform float ambient;
uniform float specular;

// Lighting
uniform PointLight[MAX_LIGHTS] lights;

// integers
uniform int nLights;
uniform int LOD;
uniform int n_levels;

// output(s)
out vec4 fragColor;

// calculate the normal from the normal map in tangent space.
vec3 getNormalInWorldSpace(){
    // retrieve value from normal map. Without an assigned normal map this is a straight up vector.
    vec3 mapNormal = texture(nMap, TexCoords).xyz;
    mapNormal = normalize(2 * (mapNormal - vec3(0.5)));

    // normalize all tangent space vectors and make a matrix from them.
    vec3 normal = normalize(fragNormal);
    vec3 tan = normalize(fragTan);
    vec3 bitan = normalize(fragBitan);

    mat3 tanSpaceMat = mat3(tan, bitan, normal);

    // transform the mapped normal with the tangent space matrix & return.
    return normalize(tanSpaceMat * mapNormal);
}

vec3 calculateFragColor(vec4 base){

    float shiny = 1.0 - roughness;
    vec3 result = ambient * base.xyz;
    // calculate normal accounting for nMap
    vec3 normal = getNormalInWorldSpace();
    // apply diffuse and specular changes for each light affecting the object.
    for(int i = 0; i < nLights; i++){
        PointLight light = lights[i];
        // diffuse:
        vec3 lightDir = normalize(light.pos - fragPos);
        float diff_brightness = 1 - floor(max(0, dot(normal, -lightDir)) * n_levels) / n_levels;
        float intensity = min(1.0, light.I);

        result += intensity * light.emission * base.xyz * diff_brightness;

        // specular
        float NdotL = dot(normal, lightDir);
        // only render if light in front of fragment
        if(NdotL > 0.0){
            vec3 view = normalize(camPos - fragPos);
            vec3 vHalf = normalize(lightDir + view);
            float shinyFactor = mix(1.0, specular, shiny);
            float angle = max(0.001, dot(normal, vHalf));
            result += intensity * light.emission * pow(angle, shinyFactor);
        }
    }
    return result;
}

void main()
{
    if(LOD == 0) {
        vec4 base_color = ambient * vec4(diffuse.xyz, alpha) * texture(image, TexCoords * tiling);
        fragColor = vec4(base_color.xyz, alpha) ;
        return;
    }

    fragColor = vec4(calculateFragColor(vec4(diffuse, 1.0)).xyz, alpha) * texture(image, TexCoords * tiling);
}
