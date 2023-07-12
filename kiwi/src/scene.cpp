#include "scene.hpp"
#include "stb_image.h"


Scene::Scene() : input_handler(mouse, keyboard), controller(camera) {

	perspective.min = 0.1f;
	perspective.max = 1000.0f;

}


bool Scene::initialize() {

	event_dispatcher_t::add_listener<mouse_moved_t>(*this);
	event_dispatcher_t::add_listener<key_pressed_t>(*this);
	event_dispatcher_t::add_listener<key_released_t>(*this);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	shader.load("assets/shaders/colors.vs",
		"assets/shaders/colors.fs");

	sun.m_position = { 0, 0, 0 };
	sun.spin_speed = 1;
	sun.orbital_speed = 0;
	sun.orbit_radius = 0;
	sun.m_scale = { 8, 8, 8 };
	sun.planet_name = "sun";
	sun.diffuse = loadTexture("assets/maps/0_diffuse.png");

	mercury.m_position = { 9,0,0 };
	mercury.spin_speed = 5;
	mercury.orbital_speed = 1;
	mercury.orbit_radius = 15;
	mercury.m_scale = { 3,3,3 };
	mercury.planet_name = "mercury";
	mercury.diffuse = loadTexture("assets/maps/1_diffuse.png");
	mercury.set_parent(sun);

	venus.m_position = { 8, 0, 0 };
	venus.spin_speed = 7;
	venus.orbital_speed = 2;
	venus.orbit_radius = 20;
	venus.m_scale = { 3, 3, 3 };
	venus.planet_name = "venus";
	venus.diffuse = loadTexture("assets/maps/2_diffuse.png");
	venus.set_parent(sun);



	earth.m_position = { 0, 0, -9 };
	earth.spin_speed = 3;
	earth.orbital_speed = 3;
	earth.orbit_radius = 27;
	earth.m_scale = { 2, 2, 2 };
	earth.planet_name = "earth";
	earth.diffuse = loadTexture("assets/maps/3_diffuse.png");
	earth.set_parent(sun);

	moon.m_position = { 0, 0, -10 };
	moon.spin_speed = 4;
	moon.orbital_speed = 10;
	moon.orbit_radius = 10;
	moon.m_scale = { 1, 1, 1 };
	moon.planet_name = "moon";
	moon.diffuse = loadTexture("assets/maps/4_diffuse.png");
	moon.set_parent(earth);

	mars.m_position = { -13, 0, 0 };
	mars.spin_speed = 2;
	mars.orbital_speed = 2.3f;
	mars.orbit_radius = 33;
	mars.m_scale = { 2, 2, 2 };
	mars.planet_name = "mars";
	mars.diffuse = loadTexture("assets/maps/5_diffuse.png");
	mars.set_parent(sun);

	saturn.m_position = { -15, 0, 0 };
	saturn.spin_speed = 1;
	saturn.orbital_speed = 0.5f;
	saturn.orbit_radius = 45;
	saturn.m_scale = { 6, 6, 6 };
	saturn.planet_name = "saturn";
	saturn.diffuse = loadTexture("assets/maps/6_diffuse.png");
	saturn.set_parent(sun);

	jupiter.m_position = { 0,0,-18 };
	jupiter.spin_speed = 7;
	jupiter.orbital_speed = 3;
	jupiter.orbit_radius = 59;
	jupiter.m_scale = { 4, 4, 4 };
	jupiter.planet_name = "jupiter";
	jupiter.diffuse = loadTexture("assets/maps/7_diffuse.png");
	jupiter.set_parent(sun);


	neptune.m_position = { 0, 0, -23 };
	neptune.spin_speed = 0.5f;
	neptune.orbital_speed = 0.2f;
	neptune.orbit_radius = 65;
	neptune.m_scale = { 4, 4, 4 };
	neptune.planet_name = "neptune";
	neptune.diffuse = loadTexture("assets/maps/8_diffuse.png");
	neptune.set_parent(sun);

	uranus.m_position = { 0,0, 27 };
	uranus.spin_speed = 0.05f;
	uranus.orbital_speed = 0.05f;
	uranus.orbit_radius = 75;
	uranus.m_scale = { 5, 5, 5 };
	uranus.planet_name = "uranus";
	uranus.diffuse = loadTexture("assets/maps/9_diffuse.png");
	uranus.set_parent(sun);



	solar_system.push_back(&sun);
	solar_system.push_back(&mercury);
	solar_system.push_back(&venus);
	solar_system.push_back(&earth);
	solar_system.push_back(&moon);
	solar_system.push_back(&mars);
	solar_system.push_back(&saturn);
	solar_system.push_back(&jupiter);
	solar_system.push_back(&neptune);
	solar_system.push_back(&uranus);



	camera.m_position = { 0, -180, 0};
	camera.rotate_x(1.6f);





	return true;
}

bool Scene::update(timespan_t delta, glm::vec2 window_size) {

	if (keyboard.pressed(GLFW_KEY_ESCAPE)) {
		running = false;
	}

	constexpr float kPI = std::numbers::pi_v<float>;
	camera.set_perspective(kPI * 0.25f,
		window_size.x / window_size.y,
		perspective.min,
		perspective.max);

	controller.update(mouse, keyboard, delta);

	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& it : solar_system) {
		it->update(delta);
	}

	draw();

	input_handler.post_frame_update();

	return running;
}

void Scene::draw() {

	shader.use();

	shader.setMat4("projection", camera.m_projection);
	shader.setMat4("view", camera.get_viewmat());
	shader.setVec3("viewPos", camera.m_position);

	shader.setVec3("light.direction", light_direction);
	shader.setVec3("light.ambient", ambient);
	shader.setVec3("light.diffuse", diffuse);
	shader.setFloat("light.shininess", 32.0f);

	for (auto& it : solar_system) {

		shader.use();
		shader.setInt("material.diffuse", 0);

		shader.setMat4("model", it->m_world);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, it->diffuse);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);


	}
}

void Scene::on_event(const mouse_moved_t& event)
{
	input_handler.on_mouse_move(event.x, event.y);
}

void Scene::on_event(const key_pressed_t& event)
{
	input_handler.on_key_pressed(event.keycode);
}
void Scene::on_event(const key_released_t& event) {
	input_handler.on_key_released(event.keycode);
}

unsigned int Scene::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format{};
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		std::cout << "Texture loaded at path: " << path << std::endl;

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
