#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "gl2d.h"

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

gl2d::Renderer2D renderer;

bool gameLogic(GLFWwindow* window) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 0.0f });

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	renderer.renderCircleOutline({ 400, 400 }, 40, Colors_White, 2, 64);

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

	while (!glfwWindowShouldClose(window)) {
		gameLogic(window);
	}

	renderer.cleanup();
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}