#include "textured_plane.h"

TexturedPlane::TexturedPlane(Shader* shader_program, Texture* texture)
    : Shape(shader_program), texture(texture)
{
    // Plane vertices - 4 corners in the XZ plane (horizontal)
    GLfloat vertex_buffer_data[] = {
        // Position coordinates (x, y, z)
        -0.5f, 0.0f, -0.5f,  // Bottom-left
         0.5f, 0.0f, -0.5f,  // Bottom-right
         0.5f, 0.0f,  0.5f,  // Top-right
        -0.5f, 0.0f,  0.5f   // Top-left
    };

    // Texture coordinates - map the entire texture to the plane
    GLfloat tex_coords[] = {
        0.0f, 0.0f,  // Bottom-left
        1.0f, 0.0f,  // Bottom-right
        1.0f, 1.0f,  // Top-right
        0.0f, 1.0f   // Top-left
    };

    // Indices - 2 triangles to form the rectangular plane
    GLuint indices[] = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(3, &buffers[0]);

    // Vertex position buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Texture coordinate buffer
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Get the uniform location for the texture sampler
    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

TexturedPlane::~TexturedPlane() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, &buffers[0]);
}

void TexturedPlane::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    // Send matrices to the shader
    Shape::draw(model, view, projection);

    // Activate and bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->getGLid());
    glUniform1i(loc_diffuse_map, 0);

    // Draw the plane (6 indices = 2 triangles)
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void TexturedPlane::key_handler(int key) {
    return;
}