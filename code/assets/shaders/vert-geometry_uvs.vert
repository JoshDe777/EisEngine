#version 460 core

// Basic vertex shader for geometry + texture. 
// Intended for use on UI objects, where mvp will only be a VP-matrix instead.
// But evidently works for other raw geometry + texture pairings too.

in vec3 aPos;           // vertex position in model space.
in vec2 texCoords;      // vertex UV coordinate

uniform mat4 mvp;       // object's model-view-projection (MVP) matrix

out vec2 TexCoords;     // [interpolated] UV coordinate in texture space (unchanged from input)

void main() {
    gl_Position = mvp * vec4(aPos.xyz, 1.0);
    TexCoords = texCoords;
}
