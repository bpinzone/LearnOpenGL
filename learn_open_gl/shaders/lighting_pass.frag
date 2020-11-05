#version 330 core

in VS_OUT {
    vec2 TexCoord;
} fs_in;

// uniform sampler2D position1;
// uniform sampler2D normal1;
uniform sampler2D albedo_spec1;

// TODO: get rid
uniform sampler2D dir_lit_viz1;

out vec4 FragColor;

// LinearizeDepth(gl_FragCoord.z) / far, gives number in [0, 1]
float LinearizeDepth(float depth) {
    /*
    We can however, transform the non-linear depth values of the fragment back to its linear sibling
    float ndc = depth * 2.0 - 1.0;  // normalized device coordinate element of [-1, 1]
    float linearDepth = (2.0 * near * far) / (far + near - ndc * (far - near));
    Returning depth values between near and far.
    */

    float z = depth * 2.0 - 1.0; // back to NDC
    // dummy values
    float near = 0.05f;
    float far = 2000.0f;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main(){

    // TODO: put back
    FragColor = vec4(texture(albedo_spec1, fs_in.TexCoord).rgb, 1);

    // float depth = texture(dir_lit_viz1, fs_in.TexCoord).r;
    // FragColor = vec4(vec3(depth), 1);


    // float normalized_depth = LinearizeDepth(depth) / 2000.0f / 2;
    // FragColor = vec4(vec3(normalized_depth), 1);
}