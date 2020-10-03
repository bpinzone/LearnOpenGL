#version 330 core


in VS_OUT {
    // In world space
    vec3 WorldNormal;
    vec3 WorldPos;
    vec2 TexCoord;
} fs_in;

// MUST output everything in world space!!!
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

struct Material {
    sampler2D diffuse1;
    sampler2D specular1;
    float shiniess;
};
uniform Material material;

void main() {
    gPosition = fs_in.WorldPos;
    gNormal = normalize(fs_in.WorldNormal);
    gAlbedoSpec.rgb = texture(material.diffuse1, fs_in.TexCoord).rgb;
    gAlbedoSpec.a = texture(material.specular1, fs_in.TexCoord).r;
}
