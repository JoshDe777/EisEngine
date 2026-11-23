#version 460 core

// This fragment shader draws objects with glassy properties, i.e. light deformation with reflection, 
//      refraction & chromatic aberration
// Any shader using this FS MUST use vertex shader vert-shader3D.vert
//      or an equivalent vertex shader outputting complex geometric assets.
// Influence from light sources is (ironically) not yet supported 
//      -> seems like it could be just blinn-phong with a fresnel instead of specular factor.
// Influence from surrounding objects is also not yet supported.
// This shader must not be called before a two-pass depth mapping shader was used for the objects to render, 
//      otherwise this may result in heavy artifacts if it doesn't break altogether 
//      (samplers point to the wrong maps or incompatible memory).
// Glassy objects will sample from a cubemap (by default the skybox) to find an object to sample.
// Supported material properties include:
//      - surface [diffuse, normal] mapping
//      - transparency [dampens the light effects]
//      - fresnel factor F0
// Supported textures include:
//      - diffuse/albedo, normal map, depth map (two-pass, through FBO)
// Required environment properties:
//      - Specular factor / Fresnel factor F0 -> Maybe make a separate property.

struct PointLight{
    vec3 pos;               // light centre in world space for illumination direction & attenuation.
    float _padding;         // required because of memory reading method according to Claude... (vec3 expected to start 16bit sequence so vec3 usage could read into the next struct)
    vec3 emission;          // light emission colour.
    float I;                // aggregated illumination strength of the light cluster.
};

// VS inputs
in vec3 fragPos;
in vec2 TexCoords;
in vec3 fragNormal;
in vec3 fragTan;
in vec3 fragBitan;

// Vec3s
uniform vec3 diffuse;
uniform vec3 eta;
uniform vec3 camPos;

// floats
uniform float alpha;
uniform float tiling;
uniform float specular;         // stand-in for Fresnel number F0
uniform float ambient;          // stand-in for the absorption factor. Should this be a material property?? Yes it should.

// integers
uniform int screenWidth;
uniform int screenHeight;

// Samplers
uniform sampler2D image;
uniform sampler2D nMap;
uniform samplerCube cubeMap;
uniform sampler2D backDepthMap;
uniform sampler2D frontDepthMap;

// output(s)
out vec4 fragColor;

vec3 getNormalInWorldSpace(){
    vec3 mapNormal = texture(nMap, TexCoords * tiling).xyz;
    mapNormal = normalize(2 * (mapNormal - vec3(0.5)));

    vec3 normal = normalize(fragNormal);
    vec3 tan = normalize(fragTan);
    vec3 bitan = normalize(fragBitan);

    mat3 tanSpaceMat = mat3(tan, bitan, normal);

    return normalize(tanSpaceMat * mapNormal);
}

vec3 calculateFragColor(vec4 base){
    // surface-mapped texture for e.g. window stickers
    vec3 texColor = texture(image, TexCoords * tiling).xyz * base.xyz;

    // depth component for transmittance effects
    vec2 screenUV = gl_FragCoord.xy / vec2(screenWidth, screenHeight);
    float zb = texture(backDepthMap, screenUV).r;
    float zf = texture(frontDepthMap, screenUV).r;

    float depth = abs(zf - zb);
    if(depth < 0.001)
        depth = 0.001;

    // establish reflection dir [reflect(-view, mapped normal)]
    vec3 normal = getNormalInWorldSpace();
    vec3 view = normalize(camPos - fragPos);
    vec3 reflectionDir = reflect(-view, normal);
    vec3 reflection = exp(-ambient * depth) * texture(cubeMap, reflectionDir).xyz;

    // refraction - chromatic aberrations by scattering the refraction angles by color channel
    vec3 rRefractDir = refract(-view, normal, eta.r);
    // guardrails for total reflection
    if(length(rRefractDir) < 0.001)
        rRefractDir = reflectionDir;
    vec3 gRefractDir = refract(-view, normal, eta.g);
    if(length(gRefractDir) < 0.001)
        gRefractDir = reflectionDir;
    vec3 bRefractDir = refract(-view, normal, eta.b);
    if(length(bRefractDir) < 0.001)
        bRefractDir = reflectionDir;

    vec3 refraction = vec3(
        texture(cubeMap, rRefractDir).r,
        texture(cubeMap, gRefractDir).g,
        texture(cubeMap, bRefractDir).b
    );
    // apply texture & tainting to the refracted light.
    refraction = refraction * exp(-ambient * depth) * texColor;

    // fresnel factor calculation
    float m = max(0, dot(view, normal));
    float F = pow((1 - m), 5) * (1 - specular) + specular;

    // reflection color calculation; blended using Fresnel approximation.
    return mix(refraction, reflection, F);
}

void main()
{
    fragColor = vec4(calculateFragColor(vec4(diffuse, 1.0)).xyz, alpha);
}
