#version 330 core

in VS_OUT {
    vec2 TexCoord;
} fs_in;

// uniform sampler2D position1;
// uniform sampler2D normal1;
uniform sampler2D albedo_spec1;

out vec4 FragColor;

void main(){

    FragColor = vec4(texture(albedo_spec1, fs_in.TexCoord).rgb, 1);
    return;
}