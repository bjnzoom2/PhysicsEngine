#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "object.h"
#include "wall.h"

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

struct gameData {
	std::vector<Object> objects = {};
	std::vector<Wall> walls = {};
};

gl2d::Renderer2D renderer;
gameData data;

bool gameLogic(GLFWwindow* window, float deltatime) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 1.0f });

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	for (int i = 0; i < data.walls.size(); i++) {
		data.walls[i].render(renderer);
	}

	for (int i = 0; i < data.objects.size(); i++) {
		data.objects[i].render(renderer);
		data.objects[i].step(deltatime, data.walls);
	}

	renderer.flush();

	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Physics Engine", NULL, NULL);

	if (!window) {
		std::cerr << "Failed to load window\n";
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGL();

	gl2d::init();
	renderer.create();

	Object obj({ 400, 400 }, { 0, 0 }, 20);
	data.objects.push_back(obj);

	Wall wall({ 200, 600 }, 0, 400);
	data.walls.push_back(wall);

	float lastframe = 0;
	float deltatime = 0;

	while (!glfwWindowShouldClose(window)) {
		float currentframe = glfwGetTime();
		deltatime = currentframe - lastframe;
		lastframe = currentframe;

		gameLogic(window, deltatime);
	}

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}