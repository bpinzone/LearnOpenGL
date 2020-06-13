#version 330 core

in vec2 TexCoords;

struct Material {
    sampler2D diffuse1;  // screen texture.
};
uniform Material material;

out vec4 FragColor;


void main() {

    // As-is
    // FragColor = texture(material.diffuse1, TexCoords);

    // Kernel post processing.
    const float c_offset = 1.0 / 300.0;
    const int c_kernel_size = 9;
    const vec2 c_offsets[c_kernel_size] = vec2[](
        vec2(-c_offset,  c_offset), // top-left
        vec2( 0.0f,    c_offset), // top-center
        vec2( c_offset,  c_offset), // top-right
        vec2(-c_offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( c_offset,  0.0f),   // center-right
        vec2(-c_offset, -c_offset), // bottom-left
        vec2( 0.0f,   -c_offset), // bottom-center
        vec2( c_offset, -c_offset)  // bottom-right
    );

    float kernel[c_kernel_size] = float[](
        // sharpen
        // -1, -1, -1,
        // -1,  9, -1,
        // -1, -1, -1

        // blur
        // 1.0 / 16, 2.0 / 16, 1.0 / 16,
        // 2.0 / 16, 4.0 / 16, 2.0 / 16,
        // 1.0 / 16, 2.0 / 16, 1.0 / 16

        // edge detection
        1, 1, 1,
        1, -8, 1,
        1, 1, 1

        // does this make any sense?
        // -1, -1, -1,
        // -1, 8, -1,
        // -1, -1, -1

    );

    vec3 tex_samples[c_kernel_size];
    for(int i = 0; i < c_kernel_size; i++) {
        tex_samples[i] = vec3(texture(material.diffuse1, TexCoords.st + c_offsets[i]));
    }
    vec3 color = vec3(0.0);
    for(int i = 0; i < c_kernel_size; i++){
        color += tex_samples[i] * kernel[i];
    }

    FragColor = vec4(color, 1.0);


    // invert
    // FragColor = vec4(vec3(1 - texture(material.diffuse1, TexCoords)), 1.0);

    // Black and white. Eye more sensitive to green, less to blue.
    // FragColor = texture(material.diffuse1, TexCoords);
    // float average = 0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
    // FragColor = vec4(average, average, average, 1.0);

}
