#pragma once
#include "system.hpp"
#include "camera.hpp"
#include "input.hpp"
#include "shader.hpp"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <numbers>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

constexpr int numb_of_planets = 1;

struct Perspective_size {
	float min,
		max;
};

namespace {

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
}


struct celestial_body {

	inline void set_parent(celestial_body& _parent) {
		parent = &_parent;
	}

	inline void calculate_world_matrix() {
		m_world = glm::mat4(1.0f);

		if (parent != nullptr) {
			m_world = glm::translate(m_world, parent->m_position);
			m_world = glm::rotate(m_world, orbital_rotation, glm::vec3(0, 1, 0));
			m_world = glm::translate(m_world, glm::vec3(orbit_radius, 0, 0));
		}
		else {
			m_world = glm::translate(m_world, glm::vec3(orbit_radius, 0, 0));
		}

		m_world = glm::rotate(m_world, rotation, glm::vec3(0, 1, 0)); 
		m_world = glm::scale(m_world, m_scale); 

		m_position = glm::vec3(m_world[3]);
	}

	inline void update(timespan_t delta) {



		rotation += spin_speed * delta.elapsed_seconds();
		orbital_rotation += orbital_speed * delta.elapsed_seconds();

		if (rotation >= 360) {
			rotation = 0;
		}

		if (orbital_rotation >= 360) {
			orbital_rotation = 0;
		}

		calculate_world_matrix();
	}
	inline void draw(Shader& shader) {

		shader.use();

		shader.setMat4("model", m_world);

		shader.setInt("material.diffuse", diffuse);


	}
	glm::vec3 m_scale = { 1,1,1 };
	glm::mat4 m_world;
	glm::vec3 m_position = { 0,0,0 };
	float orbit_radius = 10;


	std::string planet_name;
	float rotation = 0;
	float orbital_rotation = 0;
	float spin_speed = 0;
	float orbital_speed = 0;


	GLuint diffuse;

	celestial_body* parent;

};



class Scene {



private:

	GLuint VAO = 0;
	GLuint VBO = 0;

	float celestial_rotation = 0.0f;

	celestial_body sun;
	celestial_body mercury;
	celestial_body venus;
	celestial_body earth;
	celestial_body moon;
	celestial_body mars;
	celestial_body saturn;
	celestial_body jupiter;
	celestial_body neptune;
	celestial_body uranus;

	std::vector<celestial_body*> solar_system;

	mouse_t mouse;
	camera_t camera;
	first_person_camera_controller_t controller;
	keyboard_t keyboard;
	input_handler_t input_handler;

	Perspective_size perspective;
	Shader shader;

	const glm::vec3 ambient = { 0.2f, 0.2f, 0.2f };
	const glm::vec3 diffuse = { 0.2f, 0.2f, 0.2f };

	const glm::vec3 light_direction = { 1.0f, 5.0f, 2.0f };
	const float shininess = 32.0f;


	glm::vec3 scale = { 1, 1, 1 };

	unsigned int loadTexture(char const* path);


public:
	Scene();
	bool initialize();
	bool update(timespan_t delta, glm::vec2 window_size);
	void draw();

	void on_event(const mouse_moved_t& event);
	void on_event(const key_pressed_t& event);
	void on_event(const key_released_t& event);


	bool running = true;

	int screen_width = 1280, screen_height = 720;

};