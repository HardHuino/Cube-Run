#include "cube.h"
#include <iostream>

Cube::Cube(Shader* shader_program) : Shape(shader_program)
{
    // Cube vertices - 4 sommets par face = 24 sommets au total
    GLfloat vertex_buffer_data[] = {
        // Front face (z = 0.5)
        -0.5f, -0.5f,  0.5f,  // 0
         0.5f, -0.5f,  0.5f,  // 1
         0.5f,  0.5f,  0.5f,  // 2
        -0.5f,  0.5f,  0.5f,  // 3

        // Right face (x = 0.5)
         0.5f, -0.5f,  0.5f,  // 4
         0.5f, -0.5f, -0.5f,  // 5
         0.5f,  0.5f, -0.5f,  // 6
         0.5f,  0.5f,  0.5f,  // 7

         // Back face (z = -0.5)
          0.5f, -0.5f, -0.5f,  // 8
         -0.5f, -0.5f, -0.5f,  // 9
         -0.5f,  0.5f, -0.5f,  // 10
          0.5f,  0.5f, -0.5f,  // 11

          // Left face (x = -0.5)
          -0.5f, -0.5f, -0.5f,  // 12
          -0.5f, -0.5f,  0.5f,  // 13
          -0.5f,  0.5f,  0.5f,  // 14
          -0.5f,  0.5f, -0.5f,  // 15

          // Top face (y = 0.5)
          -0.5f,  0.5f,  0.5f,  // 16
           0.5f,  0.5f,  0.5f,  // 17
           0.5f,  0.5f, -0.5f,  // 18
          -0.5f,  0.5f, -0.5f,  // 19

          // Bottom face (y = -0.5)
          -0.5f, -0.5f, -0.5f,  // 20
           0.5f, -0.5f, -0.5f,  // 21
           0.5f, -0.5f,  0.5f,  // 22
          -0.5f, -0.5f,  0.5f   // 23
    };

    // Cube indices - 2 triangles par face, 6 faces
    GLuint indices[] = {
        // Front face
        0, 1, 2,
        2, 3, 0,
        // Right face
        4, 5, 6,
        6, 7, 4,
        // Back face
        8, 9, 10,
        10, 11, 8,
        // Left face
        12, 13, 14,
        14, 15, 12,
        // Top face
        16, 17, 18,
        18, 19, 16,
        // Bottom face
        20, 21, 22,
        22, 23, 20
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, &buffers[0]);

    // Vertex buffer
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Cube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);

    // 36 indices = 12 triangles = 6 faces
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

Cube::~Cube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, &buffers[0]);
}

void Cube::key_handler(int key) {
    return;
}