#include <stdio.h>
#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "objloader.hpp"
 
Mesh::Mesh(std::string file, GLuint shader)
{
	m_file = file;
	m_modelMatrix = glm::mat4(1.0f);
	m_translateMatrix = glm::mat4(1.0f);
	m_rotationMatrix = glm::mat4(1.0f);
	m_shader = shader;
	Load();
}

Mesh::~Mesh()
{
	Clear();
}

void Mesh::Clear()
{
	m_vertices.clear();
	m_uvs.clear();
	m_normals.clear();

	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_vuv);
	glDeleteVertexArrays(1, &m_vao);
}

void Mesh::DrawMesh()
{
	m_modelMatrix = m_translateMatrix * m_rotationMatrix;
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}

void Mesh::Translate(glm::vec3 position)
{
	m_translateMatrix = glm::mat4(1.0f);
	m_translateMatrix = glm::translate(m_translateMatrix, position);
}

void Mesh::Rotate(glm::vec3 vec)
{
	m_rotationMatrix = glm::rotate(m_rotationMatrix, 0.1f, vec);
	m_modelMatrix = m_translateMatrix * m_rotationMatrix;
}

void Mesh::Load()
{
	loadOBJ(m_file.c_str(), m_vertices, m_uvs, m_normals);

	m_vbo = 0;
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		m_vertices.size() * sizeof(glm::vec3),
		&m_vertices[0],
		GL_STATIC_DRAW);

	m_vuv = 0;
	glGenBuffers(1, &m_vuv);
	glBindBuffer(GL_ARRAY_BUFFER, m_vuv);
	glBufferData(
		GL_ARRAY_BUFFER,
		m_normals.size() * sizeof(glm::vec3),
		&m_normals[0],
		GL_STATIC_DRAW);

	m_vao = 0;
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);	
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_vuv);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vuv);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0);

	m_modelMatrixID = glGetUniformLocation(m_shader, "mm");
}
