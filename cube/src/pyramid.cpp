#include "pyramid.h"
#include <iostream>

Pyramid::Pyramid(Shader *shader_program, Texture* texture, glm::vec3 emissiveColor) 
    : Shape(shader_program), texture_(texture), emissiveColor_(emissiveColor)
{
    // Format: Position(3) | Normal(3) | UV(2)
    GLfloat vertex_buffer_data[] = {
        // Base (Carré)
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f,

        // Face Avant (Triangle)
         0.0f,  0.5f,  0.0f,   0.0f, 0.5f, 1.0f,    0.5f, 1.0f, // Pointe
        -0.5f, -0.5f,  0.5f,   0.0f, 0.5f, 1.0f,    0.0f, 0.0f, // Bas Gauche
         0.5f, -0.5f,  0.5f,   0.0f, 0.5f, 1.0f,    1.0f, 0.0f, // Bas Droite

        // Face Droite (Triangle)
         0.0f,  0.5f,  0.0f,   1.0f, 0.5f, 0.0f,    0.5f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.5f, 0.0f,    0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.5f, 0.0f,    1.0f, 0.0f,

        // Face Arrière (Triangle) 
         0.0f,  0.5f,  0.0f,   0.0f, 0.5f, -1.0f,   0.5f, 1.0f,
         0.5f, -0.5f, -0.5f,   0.0f, 0.5f, -1.0f,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.5f, -1.0f,   1.0f, 0.0f,

        // Face Gauche (Triangle)
         0.0f,  0.5f,  0.0f,   -1.0f, 0.5f, 0.0f,   0.5f, 1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f, 0.5f, 0.0f,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f, 0.5f, 0.0f,   1.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0, // Base 
        4, 5, 6,          // Avant
        7, 8, 9,          // Droite
        10, 11, 12,       // Arrière
        13, 14, 15        // Gauche
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal 
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
    loc_emissive_color = glGetUniformLocation(this->shader_program_, "emissiveColor");
}

void Pyramid::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{   
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);
    
    Shape::draw(model, view, projection);

    // Envoyer la couleur émissive au shader
    glUniform3f(loc_emissive_color, emissiveColor_.r, emissiveColor_.g, emissiveColor_.b);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_->getGLid());
    glUniform1i(loc_diffuse_map, 0);

    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

Pyramid::~Pyramid() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);
}

void Pyramid::key_handler(int key) {
    return;
}

glm::vec3 Pyramid::getEmissiveColor() const {
    return emissiveColor_;
}
