#version 460 core

// Vertex Shader used for non-drawing rendering, e.g. object depth mapping for glassy shaders. 
// This doesn't need properties like normals, tangents etc. so it's just 
// a fragment position estimator + distance from camera output in viewDepth.
// (meaning depth mapping does not work for surfaces created through bump maps!)

in vec3 aPos;           // vertex position in model space.

uniform mat4 mv;        // model-view matrix projecting to camera space.
uniform mat4 mvp;       // MVP matrix projecting to screen space.

out float viewDepth;    // the distance in z-direction from the camera to the object in camera space

void main()
{
    gl_Position = mvp * vec4(aPos.xyz, 1.0);
    vec4 viewPos = mv * vec4(aPos.xyz, 1.0);
    viewDepth = -viewPos.z;
}