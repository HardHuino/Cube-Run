#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

in vec3 position;
in vec2 tex_coords;

out vec2 frag_tex_coords;

void main() {
    gl_Position = projection * view * model * vec4(position, 1.0);
    frag_tex_coords = tex_coords;  // <- UTILISEZ tex_coords au lieu de position.xy
}