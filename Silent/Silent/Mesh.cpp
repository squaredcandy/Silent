#include "Mesh.h"

#include "Log.h"

void Mesh::InitBuffer()
{
	// Create and bind vertex and index data

	VAO = 0;
	VBO = 0;
	EBO = 0;

	GLuint size = sizeof(Vertex);
	CreateVAO();
	CreateVBO(size);
	CreateEBO();

	AddAttribute(0, 3, size, offsetof(Vertex, Vertex::Position));
	AddAttribute(1, 3, size, offsetof(Vertex, Vertex::Normal));
	AddAttribute(2, 2, size, offsetof(Vertex, Vertex::TexCoords));
	AddAttribute(3, 3, size, offsetof(Vertex, Vertex::Tangent));
	AddAttribute(4, 3, size, offsetof(Vertex, Vertex::Bitangent));
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindVertexArray(0);
}

void Mesh::CreateVBO(GLuint dataSize)
{
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
					&vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::CreateVBO(GLuint& vao, GLuint& vbo, 
					 GLuint64 size, void * data, GLuint drawType)
{
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::CreateEBO()
{
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint),
					&indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::AddAttribute(int attribute, int dataSize,
						GLuint dataLength, GLuint64 offset)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
 	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, 
							dataLength, (GLvoid*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::AddAttribute(GLuint& vao, GLuint& vbo, int attribute, int dataSize, 
						GLuint dataLength, GLuint64 offset)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE,
							dataLength, (GLvoid*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::AddInstancedAttribute(int attribute, int dataSize, 
								 GLuint dataLength, GLuint64 offset)
{
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
 	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, 
							dataLength, (GLvoid*) offset);
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::AddInstancedAttribute(GLuint& vao, GLuint& vbo, int attribute, 
								 int dataSize, GLuint dataLength, GLuint64 offset)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE,
							dataLength, (GLvoid*) offset);
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ProcessMesh(aiMesh * mesh)
{
	// Walk through each of the mesh's vertices
	vertices.reserve(mesh->mNumVertices);
	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{
		vertices.emplace_back(Vertex());
		auto& vertex = vertices.back();

		// Position
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// normals
		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		// tangent
		vertex.Tangent.x = mesh->mTangents[i].x;
		vertex.Tangent.y = mesh->mTangents[i].y;
		vertex.Tangent.z = mesh->mTangents[i].z;

		// bitangent
		vertex.Bitangent.x = mesh->mBitangents[i].x;
		vertex.Bitangent.y = mesh->mBitangents[i].y;
		vertex.Bitangent.z = mesh->mBitangents[i].z;
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (GLuint i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (GLuint j = 0; j < face.mNumIndices; ++j)
			indices.emplace_back(face.mIndices[j]);
	}

	InitBuffer();
}
	
void Mesh::ProcessNode(aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];	
		ProcessMesh(mesh);
	}

	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Mesh::LoadModel(std::string path)
{
	Assimp::Importer importer;
	//path.insert(0, MESH_FOLDER);
	const aiScene * scene =
		importer.ReadFile(path,
						  //aiProcessPreset_TargetRealtime_MaxQuality);
						  aiProcess_Triangulate |
						  //aiProcess_FlipUVs |
						  aiProcess_CalcTangentSpace |
// 						  aiProcess_OptimizeMeshes |
// 						  aiProcess_OptimizeGraph |
						  aiProcess_JoinIdenticalVertices |
						  aiProcess_SortByPType);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || 
		!scene->mRootNode)
	{
		LOG_ERROR("ASSIMP Error: %s\n", importer.GetErrorString());
		return;
	}
	ProcessNode(scene->mRootNode, scene);
	LOG_INFO("Model Loaded: %s\n", 
			 path.substr(path.find_last_of('/') + 1).c_str());
}

void Mesh::Cleanup()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}