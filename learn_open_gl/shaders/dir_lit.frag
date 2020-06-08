#version 330 core

// The screen space coordinates relate directly to the viewport defined by OpenGL's glViewport function and can be accessed via GLSL's built-in gl_FragCoord variable in the fragment shader

in vec3 Normal;
in vec3 WorldPos;
in vec2 TexCoords;

uniform vec3 camera_pos;

struct Material {
    // colors reflected for each light component
    sampler2D diffuse1;  // serves as diffuse AND ambient color.
    sampler2D specular1;
    float shininess;
};
uniform Material material;

struct Dir_light {

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 direction;
};
uniform Dir_light dir_light;

out vec4 FragColor;

vec3 calc_dir_light(Dir_light dir_light, vec3 normal_n, vec3 frag_to_camera_n);

// REQUIRES: dir_light.direction, normal_n, and frag_to_camera_n are normalized.
vec3 calc_dir_light(Dir_light dir_light, vec3 normal_n, vec3 frag_to_camera_n) {

    float diff = max(dot(normal_n, -dir_light.direction), 0.0);

    vec3 light_ref = normalize(reflect(dir_light.direction, normal_n));
    float spec = pow(max(dot(light_ref, frag_to_camera_n), 0.0), material.shininess);

    // todo: don't cast to vec3 if you want transparency.
    /*
    example, this vs this
    // FragColor = vec4(vec3(texture(texture1, TexCoords)), 1.0);
    // FragColor = texture(texture1, TexCoords);
    */
    vec3 ambient  = dir_light.ambient  *        vec3(texture(material.diffuse1, TexCoords));
    vec3 diffuse  = dir_light.diffuse  * diff * vec3(texture(material.diffuse1, TexCoords));
    vec3 specular = dir_light.specular * spec * vec3(texture(material.specular1, TexCoords));
    return ambient + diffuse + specular;
}

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
    float near = 999;
    float far = 999;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {

    // Visualize UV
    // FragColor = vec4(TexCoords.x, TexCoords.y, 0, 1);
    // return;

    // gl_FragCoord.z is a depth value. Just like this, it is NON linear wrt the frustum.
    // To linearize it, see the function above.
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    // FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / 800.0), 1.0);
    // return;

    vec3 norm = normalize(Normal);

    // // Normal debugging
    // FragColor = vec4(norm * 0.5 + 0.5, 1);
    // FragColor = vec4(WorldPos * 0.01 + 0.5, 1);
    // return;

    vec3 frag_to_camera_n = normalize(camera_pos - WorldPos);

    vec3 result = calc_dir_light(dir_light, norm, frag_to_camera_n);
    FragColor = vec4(result, texture(material.diffuse1, TexCoords).a);

    // How to discard. (Before learned blending)
    // if(FragColor.a < 0.1){
    //     discard;
    // }

    return;
}