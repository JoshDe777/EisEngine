#version 460 core
in vec2 TexCoords;
out vec4 fragColor;

uniform sampler2D image;
uniform vec4 color;

void main()
{
    fragColor = color * texture(image, TexCoords);
}
