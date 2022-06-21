#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Mesh
{
public:
	std::string m_file;
	Mesh(std::string file, GLuint shader);
	~Mesh();
	void Clear();
	void DrawMesh();
	void Translate(glm::vec3);
	void Rotate(glm::vec3);

private:
	std::vector<glm::vec3> m_vertices;
	std::vector<glm::vec2> m_uvs;
	std::vector<glm::vec3> m_normals;
	GLuint m_vbo;
	GLuint m_vuv;
	GLuint m_vao;
	GLuint m_modelMatrixID;
	glm::mat4 m_translateMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_modelMatrix;
	GLuint m_shader;

	void Load();
};

