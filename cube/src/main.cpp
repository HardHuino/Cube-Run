

#include "viewer.h"
#include "texture_cube.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include "Camera.h"
#include <string>

#ifndef SHADER_DIR
#error "SHADER_DIR not defined"
#endif

int main()
{
    // Create window
    Viewer viewer(800, 800);

    // Get shader directory
    std::string shader_dir = SHADER_DIR;

    // Create texture shader
    Shader* texture_shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture.frag");

    // Load 6 textures (une pour chaque face)
    Texture* tex_front = new Texture("C:\\Users\\coren\\Documents\\Polytech\\ET3\\InfoGraphic\\projet\\cube\\textures\\images.jpg");
    Texture* tex_back = new Texture("C:\\Users\\coren\\Documents\\Polytech\\ET3\\InfoGraphic\\projet\\cube\\textures\\images.jpg");
    Texture* tex_left = new Texture("C:\\Users\\coren\\Documents\\Polytech\\ET3\\InfoGraphic\\projet\\cube\\textures\\images.jpg");
    Texture* tex_right = new Texture("C:\\Users\\coren\\Documents\\Polytech\\ET3\\InfoGraphic\\projet\\cube\\textures\\images.jpg");
    Texture* tex_top = new Texture("C:\\Users\\coren\\Documents\\Polytech\\ET3\\InfoGraphic\\projet\\cube\\textures\\images.jpg");
    Texture* tex_bottom = new Texture("C:\\Users\\coren\\Documents\\Polytech\\ET3\\InfoGraphic\\projet\\cube\\textures\\images.jpg");

    // Create textured cube with 6 different textures
    Shape* cube = new TexturedCube(texture_shader,
        tex_front, tex_back,
        tex_left, tex_right,
        tex_top, tex_bottom);

    // Create transformation matrix for the cube
    glm::mat4 cube_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f))
        * glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f))
        * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(1.0f, 1.0f, 0.0f));

    // Create node and add cube to it
    Node* cube_node = new Node(cube_mat);
    cube_node->add(cube);

    // Add cube node to scene
    viewer.scene_root->add(cube_node);

    // Run the viewer
    viewer.run();

    // Cleanup
    delete cube_node;
    delete cube;
    delete tex_front;
    delete tex_back;
    delete tex_left;
    delete tex_right;
    delete tex_top;
    delete tex_bottom;
    delete texture_shader;

    return 0;
}