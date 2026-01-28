#include "viewer.h"
#include "texture_cube.h"
#include "pyramid.h"
#include "texture.h"
#include "node.h"
#include "shader.h"
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#define SHADER_DIR "shaders/"
#define TEXTURE_DIR "textures/"

#ifndef SHADER_DIR
#error "SHADER_DIR needs to be defined!"
#endif

int main()
{
    Viewer viewer(1920, 1080);

    std::string shader_dir = SHADER_DIR;
    std::string tex_dir = TEXTURE_DIR;

    // Shader pour les objets "normaux" (sol, joueur), supporte multi-lumières
    Shader* shader = new Shader(shader_dir + "texture.vert", shader_dir + "texture_multi_lights.frag");
    
    // Shader pour les pyramides émissives
    Shader* emissive_shader = new Shader(shader_dir + "emissive.vert", shader_dir + "emissive.frag");

    //Textures pour les objets
    Texture* tex_player = new Texture(tex_dir + "player_texture.jpg");
    Texture* tex_ground = new Texture(tex_dir + "ground_texture.jpg");
    Texture* tex_sky    = new Texture(tex_dir + "sky_texture.jpg");
    Texture* tex_obs    = new Texture(tex_dir + "obstacle_texture.jpg");

    //Skybox
    Shape* sky_shape = new TexturedCube(shader, tex_sky, tex_sky, tex_sky, tex_sky, tex_sky, tex_sky);
    glm::mat4 sky_mat = glm::scale(glm::mat4(1.0f), glm::vec3(800.0f, 800.0f, 800.0f));
    Node* sky_node = new Node(sky_mat);
    sky_node->add(sky_shape);
    viewer.scene_root->add(sky_node);
    viewer.sky_node = sky_node;

    //Sol
    Shape* ground_shape = new TexturedCube(shader, tex_ground, tex_ground, tex_ground, tex_ground, tex_ground, tex_ground);
    float g_len = 200.0f;
    viewer.ground_length = g_len;

    glm::mat4 g1_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -g_len/2.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(12.5f, 0.2f, g_len));
    Node* node_g1 = new Node(g1_mat);
    node_g1->add(ground_shape);

    glm::mat4 g2_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -2.0f, -g_len/2.0f - g_len)) * glm::scale(glm::mat4(1.0f), glm::vec3(12.5f, 0.2f, g_len));
    Node* node_g2 = new Node(g2_mat);
    node_g2->add(ground_shape);

    viewer.scene_root->add(node_g1);
    viewer.scene_root->add(node_g2);
    viewer.ground1 = node_g1;
    viewer.ground2 = node_g2;

    // Couleurs émissives variées pour les pyramides
    glm::vec3 pyramid_colors[] = {
        glm::vec3(1.0f, 0.2f, 0.2f),  // Rouge
        glm::vec3(0.2f, 1.0f, 0.2f),  // Vert
        glm::vec3(0.2f, 0.2f, 1.0f),  // Bleu
        glm::vec3(1.0f, 1.0f, 0.2f),  // Jaune
        glm::vec3(1.0f, 0.2f, 1.0f),  // Magenta
        glm::vec3(0.2f, 1.0f, 1.0f),  // Cyan
        glm::vec3(1.0f, 0.5f, 0.2f),  // Orange
        glm::vec3(0.5f, 0.2f, 1.0f),  // Violet
        glm::vec3(1.0f, 0.8f, 0.2f),  // Or
        glm::vec3(0.2f, 1.0f, 0.5f)   // Vert clair
    };
    
    // Créer les pyramides avec couleurs différentes
    for (int i = 0; i < 10; i++) {
        Shape* obs_shape = new Pyramid(emissive_shader, tex_obs, pyramid_colors[i]);
        
        glm::mat4 obs_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(1.5f, 2.0f, 1.5f)); 
        Node* obs_node = new Node(obs_mat);
        obs_node->add(obs_shape);
        
        viewer.scene_root->add(obs_node);
        viewer.obstacles.push_back(obs_node);
        viewer.obstacle_shapes.push_back(dynamic_cast<Pyramid*>(obs_shape));
    }

    Shape* player_shape = new TexturedCube(shader, tex_player, tex_player, tex_player, tex_player, tex_player, tex_player);
    glm::mat4 player_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    Node* player_node = new Node(player_mat);
    player_node->add(player_shape);

    viewer.scene_root->add(player_node);
    viewer.player_node = player_node;
    viewer.player_shape = player_shape;
    viewer.ground_shape = ground_shape;

    viewer.run();

    delete player_node;
    delete node_g1;
    delete node_g2;
    delete sky_node;
    delete tex_player;
    delete tex_ground;
    delete tex_sky;
    delete tex_obs;
    delete shader;
    delete emissive_shader;
    delete sky_shape;
    delete ground_shape;
    // Les pyramides sont déjà supprimées via les nodes
    delete player_shape;

    return 0;
}
