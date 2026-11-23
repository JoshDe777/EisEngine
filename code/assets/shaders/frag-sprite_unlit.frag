#version 460 core

// This fragment shader is used for simple objects with a texture and no lighting calculation.
// Programs using this FS should use the vertex shader 'vert-geometry_uvs.vert'.

in vec2 TexCoords;

uniform sampler2D image;
uniform float tiling;
uniform vec3 diffuse;

out vec4 fragColor;

void main()
{
    fragColor = vec4(diffuse.xyz, 1.0) * texture(image, TexCoords * tiling);
}
