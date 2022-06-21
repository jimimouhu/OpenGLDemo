#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.hpp"
#include "Mesh.h"

using namespace glm;
using namespace std;

int vertexCount = 0;

GLFWwindow* InitWindow() {

	// OpenGL setup //
	if (!glfwInit()) {
		fprintf(stderr, "GLFW error!");
		return NULL;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// OpenGL setup //

	// Window formation
	GLFWwindow* window;
	window = glfwCreateWindow(
		1024,
		768,
		"Transforms",
		NULL, NULL);

	if (window == NULL) {
		fprintf(stderr, "Window formation failed");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	GLenum err = glewInit();

	if (err != GLEW_OK) {
		fprintf(stderr, "%s\n", glewGetErrorString(err));
		fprintf(stderr, "GLEW FAILED!\n");
		return NULL;
	}

	return window;
}

int tehtava3(void) {
	GLFWwindow* window = InitWindow();
	if (window == NULL)
	{
		return -1;
	}

	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint programID = LoadShaders("vertex.glsl", "fragment.glsl");
	GLuint VMMatrixID = glGetUniformLocation(programID, "vm");
	GLuint PMMatrixID = glGetUniformLocation(programID, "pm");

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(
		glm::vec3(4, 3, 3),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	Mesh meshes[3] = {
		Mesh("bunny.obj", programID),
		Mesh("dino.obj", programID),
		Mesh("otherBunny.obj", programID)
	};

	meshes[0].Translate(glm::vec3(1.5f, 0, 0));
	meshes[1].Translate(glm::vec3(-1.5f, 0, 0));
	meshes[2].Translate(glm::vec3(1.5f, 0, 1.5f));

	glUseProgram(programID);

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double t = glfwGetTime();
		float z = (float)glm::sin(t) * 0.1f;

		glUniformMatrix4fv(VMMatrixID, 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(PMMatrixID, 1, GL_FALSE, &projection[0][0]);

		for (int i = 0; i < 3; i++) {
			meshes[i].DrawMesh();
		}


		meshes[0].Rotate(glm::vec3(0, z, 0));
		meshes[1].Translate(glm::vec3(0, z * 5, 0));
		meshes[2].Translate(glm::vec3(z * 5, 0, z * 5));


		glfwSwapBuffers(window);
		glfwPollEvents();

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glDeleteProgram(programID);
	glfwTerminate();
}


int main(void) {
	tehtava3();
	return 0;
}