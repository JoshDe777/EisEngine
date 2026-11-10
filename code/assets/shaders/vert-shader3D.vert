#version 460 core

// This is the default vertex shader for EisEngine rendering purposes.
// It calculates and passes to the vertex shader complex geometry (vertex position, UV, normal, tangent, bitangent)
// to enable more active surface mapping in the fragment shader.

in vec3 aPos;           // vertex position in model space.
in vec3 normal;         // vertex normal vector
in vec2 texCoords;      // vertex UV coordinate
in vec3 tan;            // vertex tangent vector
in vec3 bitan;          // vertex bitangent vector

uniform mat4 mvp;       // object's model-view-projection (MVP) matrix
uniform mat3 normalMat; // object's normal matrix (model matrix w/o scale)
uniform mat4 model;     // object's model matrix.

out vec3 fragPos;       // [interpolated] fragment position in camera space
out vec2 TexCoords;     // [interpolated] UV coordinate in texture space (unchanged from input)
out vec3 fragNormal;    // [interpolated] fragment normal in model space
out vec3 fragTan;       // [interpolated] fragment tangent in model space
out vec3 fragBitan;     // [interpolated] fragment bitangent in model space

void main()
{
    TexCoords = texCoords;
    fragNormal = normalMat * normal;
    fragTan = normalMat * tan;
    fragBitan = normalMat * bitan;
    gl_Position = mvp * vec4(aPos.xyz, 1.0);
    fragPos = (model * vec4(aPos.xyz, 1.0)).xyz;
}
