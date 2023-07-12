#include "scene.hpp"


int main(int argc, char** argv)
{

	Scene scene;


	if (!glfwInit()) {
		debug::error("could not initialize glfw!");
		return 0;
	}

	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(scene.screen_width, scene.screen_height, "Solar System", nullptr, nullptr);
	if (window == nullptr) {
		debug::error("could not create window!");
		glfwTerminate();
		return 0;
	}

	glfwMakeContextCurrent(window);
	if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == 0) {
		debug::error("could not load opengl functions!");
		return 0;
	}

	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	if (!scene.initialize()) {
		return 0;
	}


	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
		event_dispatcher_t::dispatch(mouse_moved_t{ int(xpos), int(ypos) });
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			event_dispatcher_t::dispatch(key_pressed_t{ key });
		}
		if (action == GLFW_RELEASE) {
			event_dispatcher_t::dispatch(key_released_t{ key });
		}
	});



	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videomode = glfwGetVideoMode(monitor);
	const float monitor_refresh_rate = float(videomode->refreshRate);

	float frames_per_second = monitor_refresh_rate;
	timespan_t time_last_frame;
	timespan_t average_frame_duration = timespan_t::from_seconds(1.0f / frames_per_second);



	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {


		const timespan_t current_time = watch_t::time_since_start();
		const timespan_t current_frame_duration = current_time - time_last_frame;
		const float current_frames_per_second = 1.0f / current_frame_duration.elapsed_seconds();
		time_last_frame = current_time;

		constexpr float leaky_factor_from = 0.98f;
		constexpr float leaky_factor_to = 1.0f - leaky_factor_from;
		frames_per_second = frames_per_second * leaky_factor_from + current_frames_per_second * leaky_factor_to;
		average_frame_duration = average_frame_duration * leaky_factor_from + current_frame_duration * leaky_factor_to;


		glfwPollEvents();

		int window_width = 0, window_height = 0;
		glfwGetWindowSize(window, &window_width, &window_height);

		if (!scene.update(current_frame_duration, glm::vec2(float(window_width), float(window_height)))) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		glfwSwapBuffers(window);

		{
			char title[256];
			sprintf_s(title,
				"Solar System - window: %dx%d fps: %2.3f frame: %2.3fms",
				window_width,
				window_height,
				frames_per_second,
				average_frame_duration.elapsed_milliseonds());
			glfwSetWindowTitle(window, title);
		}
	}



	return 0;

}
