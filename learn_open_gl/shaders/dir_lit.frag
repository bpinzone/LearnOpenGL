#version 330 core

/*

More useful built in variables.
https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL

gl_PointSize
gl_VertexID
gl_FragCoord
gl_FrontFacing
gl_FragDepth

*/

// The screen space coordinates relate directly to the viewport defined by OpenGL's glViewport function and can be accessed via GLSL's built-in gl_FragCoord variable in the fragment shader

in VS_OUT {
    vec3 Normal;
    vec3 WorldPos;
    vec2 TexCoords;
} fs_in;

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

    // phong
    // float spec = pow(max(dot(light_ref, frag_to_camera_n), 0.0), material.shininess);

    // blinn-phong
    vec3 halfway = normalize(normalize(dir_light.direction) + frag_to_camera_n);
    float spec = 0;
    if(diff > 0){
        spec = pow(max(dot(halfway, normalize(fs_in.Normal)), 0.0), material.shininess);
    }

    // todo: don't cast to vec3 if you want transparency.
    // Actually: This is being delt with below. (Adding transparency component)
    /*
    example, this vs this
    // FragColor = vec4(vec3(texture(texture1, fs_in.TexCoords)), 1.0);
    // FragColor = texture(texture1, fs_in.TexCoords);
    */
    vec3 ambient  = dir_light.ambient  *        vec3(texture(material.diffuse1, fs_in.TexCoords));
    vec3 diffuse  = dir_light.diffuse  * diff * vec3(texture(material.diffuse1, fs_in.TexCoords));
    vec3 specular = dir_light.specular * spec * vec3(texture(material.specular1, fs_in.TexCoords));
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

    // dummy values. Near Far clipping plane point of maintenance.
    float near = 999;
    float far = 999;
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {

    // Visualize UV
    // FragColor = vec4(fs_in.TexCoords.x, fs_in.TexCoords.y, 0, 1);
    // return;

    // gl_FragCoord.z is a depth value. Just like this, it is NON linear wrt the frustum.
    // To linearize it, see the function above.
    // FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    // FragColor = vec4(vec3(LinearizeDepth(gl_FragCoord.z) / 800.0), 1.0);
    // return;

    vec3 norm = normalize(fs_in.Normal);

    // // Normal debugging
    // FragColor = vec4(norm * 0.5 + 0.5, 1);
    // FragColor = vec4(fs_in.WorldPos * 0.01 + 0.5, 1);
    // return;

    vec3 frag_to_camera_n = normalize(camera_pos - fs_in.WorldPos);

    vec3 result = calc_dir_light(dir_light, norm, frag_to_camera_n);
    FragColor = vec4(result, texture(material.diffuse1, fs_in.TexCoords).a);

    // How to discard. (Before learned blending)
    // if(FragColor.a < 0.1){
    //     discard;
    // }

    return;
}