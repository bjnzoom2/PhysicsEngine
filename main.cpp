#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "object.h"
#include "wall.h"
#include "spring.h"
#include <imgui.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

unsigned int windowWidth = 800;
unsigned int windowHeight = 800;

struct gameData {
	std::vector<Object> objects = {};
	std::vector<Wall> walls = {};
	std::vector<Spring> springs = {};
	glm::dvec2 cursorPos = {};

	glm::dvec2 wallStartPos = {};

	bool QKeyState = false;
	float timer;
	float restitution = 0.9;
	float friction = 0.001;
};

gl2d::Renderer2D renderer;
gameData data;

bool gameLogic(GLFWwindow* window, float deltatime) {
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT);
	renderer.updateWindowMetrics(windowWidth, windowHeight);
	renderer.clearScreen({ 0.0f, 0.0f, 0.0f, 1.0f });
	data.timer += deltatime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && data.timer > 0.2) {
		glfwGetCursorPos(window, &data.cursorPos.x, &data.cursorPos.y);
		Object obj(data.cursorPos, {}, 20);
		data.objects.push_back(obj);
		data.timer = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS && data.timer > 0.2) {
		glfwGetCursorPos(window, &data.cursorPos.x, &data.cursorPos.y);
		if (!data.QKeyState) {
			data.wallStartPos = data.cursorPos;
		}
		else {
			Wall wall(data.wallStartPos, data.cursorPos);
			data.walls.push_back(wall);
		}

		data.QKeyState = !data.QKeyState;
		data.timer = 0;
	}

	if (data.QKeyState) {
		glfwGetCursorPos(window, &data.cursorPos.x, &data.cursorPos.y);
		renderer.renderLine(data.wallStartPos, data.cursorPos, Colors_White, 5);
	}

	std::cout << data.QKeyState << '\n';

	for (int i = 0; i < data.objects.size(); i++) {
		data.objects[i].render(renderer);
		data.objects[i].step(deltatime, data.walls, data.objects, data.friction, data.restitution);
	}

	for (int i = 0; i < data.walls.size(); i++) {
		data.walls[i].render(renderer);
	}

	for (int i = 0; i < data.springs.size(); i++) {
		data.springs[i].render(renderer);
		data.springs[i].step(deltatime);
	}

	renderer.flush();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Debug");
	ImGui::Text("Object count: %d", (int)data.objects.size());
	ImGui::Text("Wall count: %d", (int)data.walls.size());
	ImGui::Text("Spring count: %d", (int)data.springs.size());

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Wall wall({ 0, 600 }, 0, 800);
	data.walls.push_back(wall);

	// Spring spr(data.objects[0], data.objects[1], 100, 1);
	// data.springs.push_back(spr);

	float lastframe = 0;
	float deltatime = 0;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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