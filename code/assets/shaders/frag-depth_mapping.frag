#version 460 core

// This fragment shader is used before glassy shaders to establish object depth, 
//      for accurate refraction and volume representation.
// Programs using this FS MUST use the vertex shader vert-depth_mapping.vert
// This shader is intended to draw depth maps to a framebuffer object (FBO), therefore does not require material property.

in float viewDepth;

out float outDepth;

void main(){
    outDepth = viewDepth;
}