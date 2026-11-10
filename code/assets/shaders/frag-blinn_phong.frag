#version 460 core

// This fragment shader draws the scene using Blinn-Phong illumination models.
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

// vec3s
uniform vec3 diffuse;       // object base colour [RGB].
uniform vec3 camPos;        // camera position in world space for view vectors (specularity).

// floats                   // -------------- MATERIAL DATA --------------
uniform float tiling;       // tiling factor for texture repetition.
uniform float alpha;        // object transparency
uniform float roughness;    // object roughness; inversely proportional to shininess.
                            // -------------- WORLD DATA --------------
uniform float ambient;      // ambient lighting level. applied as base illumination before complex calculation.
uniform float specular;     // specular factor influencing the sharpness of the BRDF's specular lobe.

// sampler2Ds
uniform sampler2D image;    // diffuse map/texture applied to the geometry (alters colour).
uniform sampler2D nMap;     // normal map/texture applied to the geometry (alters light effect directions).

// Lighting-related Input
layout(std430, binding=0) readonly buffer LightBuffer {
    PointLight[] lights;    // array/list of light clusters effecting on the object.
};
uniform int nLights;        // amount of effecting clusters. Will render in ambient only if set to 0.

// Output(s)
out vec4 fragColor;         // fragment colour after illumination & texturing.

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
        // calculate distance between light src and obj.
        float dist = max(distance(light.pos, fragPos), 0.01);
        float attenuation = light.I / (dist*dist);
        result += light.emission * base.xyz * max(0, dot(normal, lightDir)) * attenuation;

        // specular:
        float NdotL = dot(normal, lightDir);
        // only render specular lobe if light in front of fragment
        // applied as a mask rather than branching - if NdotL <= 0 -> mask = 0
        // result += 0*[the rest of the computation]
        float mask = step(0.0, NdotL);

        vec3 view = normalize(camPos - fragPos);
        vec3 vHalf = normalize(lightDir + view);
        float shinyFactor = mix(1.0, specular, shiny);
        float angle = max(0.001, dot(normal, vHalf));
        result += light.emission * pow(angle, shinyFactor) * attenuation * mask;
    }
    return result;
}

void main()
{
    fragColor = vec4(calculateFragColor(vec4(diffuse, 1.0)).xyz, alpha) * texture(image, TexCoords * tiling);
}
