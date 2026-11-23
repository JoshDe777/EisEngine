#version 460 core

// This fragment shader is used to draw a skybox to the background of the scene. 
// The skybox does not interact with light in the scene, therefore needs no material properties. 
// Any program using this FS MUST also use the vertex shader 'vert-skybox.vert'.

in vec3 TexCoords;

uniform samplerCube cubeMap;

out vec4 fragColor;

void main(){
    fragColor = vec4(texture(cubeMap, TexCoords).xyz, 1.0);
}
