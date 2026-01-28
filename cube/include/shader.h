#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>


class Shader {
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);

    ~Shader();

    GLuint get_id();

private:
    GLuint glid;
    GLuint compile_shader(const std::string& path, GLenum shader_type);
};

#endif // SHADER_H
