#include "texture_cube.h"

TexturedCube::TexturedCube(Shader* shader_program,
    Texture* tex_front, Texture* tex_back,
    Texture* tex_left, Texture* tex_right,
    Texture* tex_top, Texture* tex_bottom)
    : Shape(shader_program)
{
    textures[0] = tex_front; textures[1] = tex_right; textures[2] = tex_back;
    textures[3] = tex_left;  textures[4] = tex_top;   textures[5] = tex_bottom;

    // LE CHANGEMENT EST ICI : 
    // On ajoute 3 chiffres au milieu de chaque ligne pour la Normale (nx, ny, nz)
    // Format : Position(3), NORMAL(3), Texture(2)
    GLfloat vertex_buffer_data[] = {
        // Front face (Normale vers nous : 0, 0, 1)
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,

        // Right face (Normale vers la droite : 1, 0, 0)
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

         // Back face (Normale vers le fond : 0, 0, -1)
          0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,   0.0f, 0.0f, -1.0f,   0.0f, 1.0f,

          // Left face (Normale vers la gauche : -1, 0, 0)
          -0.5f, -0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
          -0.5f, -0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
          -0.5f,  0.5f,  0.5f,  -1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
          -0.5f,  0.5f, -0.5f,  -1.0f, 0.0f, 0.0f,   0.0f, 1.0f,

          // Top face (Normale vers le haut : 0, 1, 0)
          -0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
           0.5f,  0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
           0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
          -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

          // Bottom face (Normale vers le bas : 0, -1, 0)
          -0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 0.0f,
           0.5f, -0.5f, -0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 0.0f,
           0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   1.0f, 1.0f,
          -0.5f, -0.5f,  0.5f,   0.0f, -1.0f, 0.0f,   0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 2, 2, 3, 0,       4, 5, 6, 6, 7, 4,       8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(2, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data), vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // CONFIGURATION DES ATTRIBUTS (Très important pour matcher le Shader)
    // 1. Position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // 2. Normal (location = 1) -> On saute 3 floats pour arriver aux normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3. Texture (location = 2) -> On saute 6 floats pour arriver aux UVs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    loc_diffuse_map = glGetUniformLocation(this->shader_program_, "diffuse_map");
}

TexturedCube::~TexturedCube() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(2, buffers);
}

void TexturedCube::draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
    glUseProgram(this->shader_program_);
    glBindVertexArray(VAO);

    Shape::draw(model, view, projection);
    
    // ENVOI DE LA LUMIERE AU SHADER
    // On place la lumière en haut à droite (World Space)
    glUniform3f(glGetUniformLocation(shader_program_, "lightPos"), 5.0f, 10.0f, 5.0f);
    // Couleur blanche
    glUniform3f(glGetUniformLocation(shader_program_, "lightColor"), 1.0f, 1.0f, 1.0f);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(loc_diffuse_map, 0);

    for(int i=0; i<6; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]->getGLid());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(GLuint)));
    }
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void TexturedCube::key_handler(int key) { return; }