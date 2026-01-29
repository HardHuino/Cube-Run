#include "viewer.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include "glm/ext.hpp"
#include <glm/gtc/matrix_transform.hpp>

Viewer::Viewer(int width, int height) : width_(width), height_(height)
{
    std::srand(std::time(nullptr));

    camera = new Camera(width, height, glm::vec3(0.0f, 0.0f, 5.0f));
    
    player_node = nullptr;
    player_shape = nullptr;
    ground_shape = nullptr;
    ground1 = nullptr;
    ground2 = nullptr;
    sky_node = nullptr;
    ground_length = 200.0f;
    
	// Vitesse de course
	run_speed = 30.0f;           // Vitesse initiale
	speed_increment = 0.01f;    // Accélération par frame

    // Configuration des lignes
    num_lanes = 8;
    lane_width = 11.0f / (num_lanes - 1); // Espacement entre -5.5 et +5.5
    current_lane = num_lanes / 2; // Commencer au milieu (ligne 4)
    target_lane = current_lane;
    is_moving = false;
    lane_transition_time = 0.0f;
    lane_transition_duration = 0.15f; // 150ms pour changer de ligne
    // 

    // Initialisation de la rotation
    player_rotation_z = 0.0f;
    target_rotation_z = 0.0f;
    start_rotation_z = 0.0f;
    // 

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        glfwTerminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    win = glfwCreateWindow(width, height, "Cube Run - Score: 0", NULL, NULL);

    if (win == NULL) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(win);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    glfwSetWindowUserPointer(win, this);
    glfwSetKeyCallback(win, key_callback_static);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    scene_root = new Node();
}

// Convertir numéro de ligne en position X
float Viewer::get_lane_x_position(int lane_index) {
    // Ligne 0 = -5.5, Ligne 7 = +5.5
    return -5.5f + (lane_index * lane_width);
}

// Obtenir une ligne aléatoire
int Viewer::get_random_lane() {
    return std::rand() % num_lanes;
}
// 

void Viewer::reset_game() {
    if (player_node) {
        glm::vec3 current_pos = player_node->getPosition();
        player_node->translate(-current_pos);

        // Réinitialiser la ligne du joueur
        current_lane = num_lanes / 2;
        target_lane = current_lane;
        is_moving = false;
        lane_transition_time = 0.0f;

        // Réinitialiser la rotation
        player_rotation_z = 0.0f;
        target_rotation_z = 0.0f;
        start_rotation_z = 0.0f;

		// Reinitialiser vitesse
		run_speed = 30.0f;
    }

    glfwSetWindowTitle(win, "Cube Run - Score: 0");

    //Remet ground a sa position initial
    if (ground1) {
        glm::vec3 g1_pos = ground1->getPosition();
        ground1->translate(glm::vec3(0, -0.75f, -ground_length/2.0f) - g1_pos); 
    }
    if (ground2) {
        glm::vec3 g2_pos = ground2->getPosition();
        ground2->translate(glm::vec3(0, -0.75f, -ground_length/2.0f - ground_length) - g2_pos);
    }

    // Placer les obstacles sur des lignes aléatoires 
    for (int i = 0; i < obstacles.size(); i++) {
        Node* obs = obstacles[i];
        glm::vec3 obs_pos = obs->getPosition();

        int random_lane = get_random_lane();
        float obs_x = get_lane_x_position(random_lane);
        float start_z = -50.0f - (i * 30.0f);

        glm::vec3 new_pos(obs_x, 0.5f, start_z);
        obs->translate(new_pos - obs_pos);
    }
}

void Viewer::update_pyramid_lights(glm::mat4& view) {
    // Collecter les positions et couleurs des pyramides
    std::vector<glm::vec3> pyramid_positions;
    std::vector<glm::vec3> pyramid_colors;
    
    for (int i = 0; i < obstacles.size(); i++) {
        glm::vec3 world_pos = obstacles[i]->getPosition();
        
        // Transformer en view space (comme dans le shader)
        glm::vec4 view_pos = view * glm::vec4(world_pos, 1.0);
        pyramid_positions.push_back(glm::vec3(view_pos));
        
        if (i < obstacle_shapes.size()) {
            pyramid_colors.push_back(obstacle_shapes[i]->getEmissiveColor());
        }
    }
    
    // Envoyer les lumières aux shaders des objets qui reçoivent la lumière
    if (player_shape) {
        GLuint shader_id = player_shape->shader_program_;
        glUseProgram(shader_id);
        
        glUniform1i(glGetUniformLocation(shader_id, "numPyramidLights"), pyramid_positions.size());
        
        for (int i = 0; i < pyramid_positions.size(); i++) {
            std::string pos_name = "pyramidLightPositions[" + std::to_string(i) + "]";
            std::string col_name = "pyramidLightColors[" + std::to_string(i) + "]";
            
            glUniform3f(glGetUniformLocation(shader_id, pos_name.c_str()), 
                       pyramid_positions[i].x, pyramid_positions[i].y, pyramid_positions[i].z);
            glUniform3f(glGetUniformLocation(shader_id, col_name.c_str()), 
                       pyramid_colors[i].r, pyramid_colors[i].g, pyramid_colors[i].b);
        }
    }
    
    if (ground_shape) {
        GLuint shader_id = ground_shape->shader_program_;
        glUseProgram(shader_id);
        
        glUniform1i(glGetUniformLocation(shader_id, "numPyramidLights"), pyramid_positions.size());
        
        for (int i = 0; i < pyramid_positions.size(); i++) {
            std::string pos_name = "pyramidLightPositions[" + std::to_string(i) + "]";
            std::string col_name = "pyramidLightColors[" + std::to_string(i) + "]";
            
            glUniform3f(glGetUniformLocation(shader_id, pos_name.c_str()), 
                       pyramid_positions[i].x, pyramid_positions[i].y, pyramid_positions[i].z);
            glUniform3f(glGetUniformLocation(shader_id, col_name.c_str()), 
                       pyramid_colors[i].r, pyramid_colors[i].g, pyramid_colors[i].b);
        }
    }
    
    glUseProgram(0);
}

void Viewer::run()
{
    float last_time = (float)glfwGetTime();
    int old_score = 0;

    reset_game();

    while (!glfwWindowShouldClose(win))
    {

        camera->Inputs(win); //Sert pour bouger la camera
        float current_time = (float)glfwGetTime();
        float dt = current_time - last_time;
        last_time = current_time;

        if (player_node != nullptr)
        {
             // MOUVEMENT EN Z
            run_speed += speed_increment;
            player_node->translate(glm::vec3(0.0f, 0.0f, -run_speed * dt));

            // Déplacement par lignes avec animation
            glm::vec3 p_pos = player_node->getPosition();

            if (is_moving) {
                // Animation en cours
                lane_transition_time += dt;
                float progress = std::min(lane_transition_time / lane_transition_duration, 1.0f);

                // Interpolation fluide (ease-out)
                float eased_progress = 1.0f - (1.0f - progress) * (1.0f - progress);

                // Animation de translation
                float start_x = get_lane_x_position(current_lane);
                float target_x = get_lane_x_position(target_lane);
                float new_x = start_x + (target_x - start_x) * eased_progress;

                // Déplacer vers la nouvelle position X
                player_node->translate(glm::vec3(new_x - p_pos.x, 0.0f, 0.0f));

                // Animation de rotation - Interpoler entre start et la position souhaité
                float new_rotation = start_rotation_z + (target_rotation_z - start_rotation_z) * eased_progress;

                // Appliquer la rotation au cube
                player_node->setRotationZ(new_rotation);
                player_rotation_z = new_rotation;

                // Fin de l'animation
                if (progress >= 1.0f) {
                    is_moving = false;
                    current_lane = target_lane;
                    lane_transition_time = 0.0f;
                    player_rotation_z = target_rotation_z; // S'assurer que la rotation finale est exacte
                    player_node->setRotationZ(player_rotation_z);
                }
            }
            // 

            // Score
            int current_score = (int)(std::abs(p_pos.z) / 10.0f);

            if (current_score > old_score) {
                std::string title = "Cube Run - Score: " + std::to_string(current_score);
                glfwSetWindowTitle(win, title.c_str());
                old_score = current_score;
            }

            // Repositionnement des obstacles sur des lignes
            for (Node* obs : obstacles) {
                glm::vec3 o_pos = obs->getPosition();

                if (o_pos.z > p_pos.z + 25.0f) {
                    float new_z = p_pos.z - 200.0f - (std::rand() % 50);
                    int random_lane = get_random_lane();
                    float new_x = get_lane_x_position(random_lane);

                    glm::vec3 target_pos(new_x, 0.35f, new_z);
                    obs->translate(target_pos - o_pos);
                }

                // Collision (vérifier si sur la même ligne)
                float dist_x = std::abs(p_pos.x - o_pos.x);
                float dist_z = std::abs(p_pos.z - o_pos.z);

                if (dist_x < 0.9f && dist_z < 0.9f) {
                    std::cout << "Game Over! Score Final: " << current_score << std::endl;
                    reset_game();
                    old_score = 0;
                    p_pos = player_node->getPosition();
                }
            }


            // --- CAMERA --- //On peut commenter cela pour bouger la camera
            camera->Position = glm::vec3(0.0f, 5.0f, p_pos.z + 10.0f);
            camera->Orientation = glm::normalize(glm::vec3(0.0f, -0.4f, -1.0f)); 

            // --- CIEL ---
            if (sky_node) {
                glm::vec3 sky_pos = sky_node->getPosition();
                sky_node->translate(p_pos - sky_pos);
            }

            // --- SOL ---
            if (ground1 && ground2) {
                glm::vec3 g1_pos = ground1->getPosition();
                glm::vec3 g2_pos = ground2->getPosition();

                if (p_pos.z < g1_pos.z - ground_length)
                    ground1->translate(glm::vec3(0.0f, 0.0f, -2 * ground_length));
                
                if (p_pos.z < g2_pos.z - ground_length)
                    ground2->translate(glm::vec3(0.0f, 0.0f, -2 * ground_length));
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::lookAt(camera->Position, camera->Position + camera->Orientation, camera->Up);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width_ / height_, 0.1f, 1000.0f);

        // Mettre à jour les lumières des pyramides avant de dessiner
        update_pyramid_lights(view);

        scene_root->draw(model, view, projection);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    delete camera;
    glfwTerminate();
}

void Viewer::key_callback_static(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Viewer* viewer = static_cast<Viewer*>(glfwGetWindowUserPointer(window));
    viewer->on_key(key);
}

// Gestion des touches pour changement de ligne 
void Viewer::on_key(int key)
{
    if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)
    {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    // Déplacement par lignes uniquement si pas déjà en mouvement
    if (!is_moving) {
        if (key == GLFW_KEY_LEFT && target_lane > 0) {
            target_lane--;
            is_moving = true;
            lane_transition_time = 0.0f;
            // Sauvegarder la rotation actuelle comme point de départ
            start_rotation_z = player_rotation_z;
            // Rotation dans le sens inverse des aiguilles d'une montre (antihoraire)
            target_rotation_z += 90.0f;
        }
        else if (key == GLFW_KEY_RIGHT && target_lane < num_lanes - 1) {
            target_lane++;
            is_moving = true;
            lane_transition_time = 0.0f;
            // Sauvegarder la rotation actuelle comme point de départ
            start_rotation_z = player_rotation_z;
            // Rotation dans le sens des aiguilles d'une montre (horaire)
            target_rotation_z -= 90.0f;
        }
    }
}
