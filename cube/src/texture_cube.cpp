#include "texture_cube.h"

TexturedCube::TexturedCube(Shader* shader_program,
    Texture* tex_front, Texture* tex_back,
    Texture* tex_left, Texture* tex_right,
    Texture* tex_top, Texture* tex_bottom)
    : Shape(shader_program)
{
    textures[0] = tex_front;
    textures[1] = tex_right;
    textures[2] = tex_back;
    textures[3] = tex_left;
    textures[4] = tex_top;
    textures[5] = tex_bottom;

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

    // Coordonnées de texture - 4 par face, chaque face utilise toute la texture (0,0 à 1,1)
    GLfloat tex_coords[] = {
        // Front face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Right face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Back face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Left face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Top face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,

        // Bottom face
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Indices - 2 triangles par face, 6 faces
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

    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

TexturedCube::~TexturedCube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(3, &buffers[0]);
}

void TexturedCube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    // Envoyer les matrices
    Shape::draw(model, view, projection);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(loc_diffuse_map, 0);

    // Dessiner chaque face avec sa propre texture
    // Front face (indices 0-5)
    glBindTexture(GL_TEXTURE_2D, textures[0]->getGLid());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    // Right face (indices 6-11)
    glBindTexture(GL_TEXTURE_2D, textures[1]->getGLid());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(GLuint)));

    // Back face (indices 12-17)
    glBindTexture(GL_TEXTURE_2D, textures[2]->getGLid());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(GLuint)));

    // Left face (indices 18-23)
    glBindTexture(GL_TEXTURE_2D, textures[3]->getGLid());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(18 * sizeof(GLuint)));

    // Top face (indices 24-29)
    glBindTexture(GL_TEXTURE_2D, textures[4]->getGLid());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(24 * sizeof(GLuint)));

    // Bottom face (indices 30-35)
    glBindTexture(GL_TEXTURE_2D, textures[5]->getGLid());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(30 * sizeof(GLuint)));

    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void TexturedCube::key_handler(int key) {
    return;
}