#pragma once

#include "Resource.h"

#include <vector>
#include <map>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/gl3w.h>
#include <glm/gtc/type_ptr.hpp>

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

using MeshID = unsigned int;

class Mesh : public Resource
{
private:
	GLuint VAO, VBO, EBO;

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

public:
	Mesh() = default;
	Mesh(std::string path) { LoadModel(path); }
	virtual ~Mesh() = default;

private:
	void InitBuffer();
	void ProcessMesh(aiMesh * mesh);
	void ProcessNode(aiNode * node, const aiScene * scene);
	void CreateVAO();
	void CreateVBO(GLuint dataSize);
	void CreateEBO();

public:
	void CreateVBO(GLuint& vao, GLuint& vbo, GLuint64 size,
				   void * data, GLuint drawType);
	void AddAttribute(int attribute, int dataSize,
					  GLuint dataLength, GLuint64 offset);
	void AddAttribute(GLuint& vao, GLuint& vbo, int attribute, int dataSize,
					  GLuint dataLength, GLuint64 offset);
	void AddInstancedAttribute(int attribute, int dataSize,
							   GLuint dataLength, GLuint64 offset);
	void AddInstancedAttribute(GLuint& vao, GLuint& vbo, int attribute,
							   int dataSize, GLuint dataLength, GLuint64 offset);

	void LoadModel(std::string path);
	void Cleanup() override;
};