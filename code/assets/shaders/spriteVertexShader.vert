#version 460 core
layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texCoords;
uniform mat4 mvp;
out vec2 TexCoords;

void main()
{
    TexCoords = texCoords;
    gl_Position = mvp * vec4(vertex.xyz, 1.0);
}
