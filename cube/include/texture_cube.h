#ifndef TEXTURED_CUBE_H
#define TEXTURED_CUBE_H

#include "shape.h"
#include "texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class TexturedCube : public Shape {
public:
    TexturedCube(Shader* shader_program,
        Texture* tex_front,
        Texture* tex_back,
        Texture* tex_left,
        Texture* tex_right,
        Texture* tex_top,
        Texture* tex_bottom);

    ~TexturedCube();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key);

private:
    GLuint VAO;
    GLuint buffers[3]; // 0: vertices, 1: tex_coords, 2: indices
    Texture* textures[6]; // 0:front, 1:right, 2:back, 3:left, 4:top, 5:bottom
    GLint loc_diffuse_map;
};

#endif