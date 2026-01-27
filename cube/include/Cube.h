#ifndef CUBE_H
#define CUBE_H

#include "shape.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Cube : public Shape {
public:
    Cube(Shader* shader_program);
    ~Cube();

    void draw(glm::mat4& model, glm::mat4& view, glm::mat4& projection) override;
    void key_handler(int key) override;

protected:
    GLuint VAO;
    GLuint buffers[2]; // 0: vertex buffer, 1: index buffer
};

#endif