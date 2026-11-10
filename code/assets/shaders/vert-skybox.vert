#version 460 core

// This vertex shader is intended specifically and exclusively for Skybox objects, since they do not use the model matrix.
// Calculates the position based on the camera's view-projection matrix, and uses vertex position as a 3D-UV coordinate.

in vec3 aPos;           // vertex position in model space.

uniform mat4 vp;        // view-projection matrix projecting from world to screen space.

out vec3 TexCoords;     // [interpolated] fragment UV coordinate in 3D texture space.

void main(){
    vec4 pos = vp * vec4(aPos.xyz, 1.0);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    TexCoords = aPos.xyz;
}