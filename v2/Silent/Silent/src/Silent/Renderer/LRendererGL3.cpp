#include "LRendererGL3.h"
#include "../Layer/LLog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GL/gl3w.c>
#include <sstream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mutex>

std::mutex shader_mutex;
std::mutex buffer_mutex;
std::mutex mesh_mutex;
std::mutex texture_mutex;

using namespace Silent;

void APIENTRY glErrorCallback(GLenum source, GLenum type,
							  GLuint id, GLenum severity,
							  GLsizei length, const GLchar * message,
							  const void * userParam)
{
	std::string sev;
	if (severity == GL_DEBUG_SEVERITY_LOW)
		sev = "DEBUG SEVERITY LOW";
	else if (severity == GL_DEBUG_SEVERITY_MEDIUM)
		sev = "DEBUG SEVERITY MEDIUM";
	else if (severity == GL_DEBUG_SEVERITY_HIGH)
		sev = "DEBUG SEVERITY HIGH";
	else if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		sev = "DEBUG SEVERITY NOTIFICATION";
	fprintf(stderr, "%s\n%s\n", sev.c_str(), message);
	if (severity == GL_DEBUG_SEVERITY_HIGH)
	{
		std::cout << (char) 7 << std::endl;
		std::cin.get();
	}
}
LRendererGL3::LRendererGL3() : LRenderer(SL_RENDERER_GL3)
{

}
void LRendererGL3::OnInitialize()
{
	gl3wInit();

	// Enable Face Culling
	glEnable(GL_CULL_FACE);

	// Enable alpha blending for textures
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_ONE, GL_ONE);

	// Enable multisampling for our game
	glEnable(GL_MULTISAMPLE);

	// Enable Depth Testing for ordering primitives
	glEnable(GL_DEPTH_TEST);

	// Enable Debug output
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(glErrorCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 
						  nullptr, false);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, false);
	//glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	//glPatchParameteri(GL_PATCH_VERTICES, 3);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);


}
void LRendererGL3::OnUpdate()
{
}
void LRendererGL3::OnDestroy()
{
	// Destory call the resource stuff

	// Destroy Shaders
	for (const auto& s : shaders)
	{
		glDeleteBuffers(1, &s._shaderID);
	}
	// Destroy Buffers
	for (const auto& b : buffers)
	{
		glDeleteBuffers(1, &b.framebuffer);
		glDeleteBuffers(1, &b.colorbuffer);
		glDeleteBuffers(1, &b.depthbuffer);
	}
	// Destroy Meshes
	for (const auto& m : meshes)
	{
		glDeleteVertexArrays(1, &m.VAO);
		glDeleteBuffers(1, &m.VBO);
		glDeleteBuffers(1, &m.EBO);
		glDeleteBuffers(1, &m.iVBO);
	}
	// Destroy Textures
	for (const auto& t : textures)
	{
		glDeleteTextures(1, &t.textureID);
	}

	shaders.clear();
	buffers.clear();
	meshes.clear();
	textures.clear();
}
void LRendererGL3::Clear(glm::vec4 col)
{
	glViewport(0, 0, size.x, size.y);
	glClearColor(col.r, col.g, col.b, col.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Shader
inline ShaderID GenerateShaderID()
{
	// ID = 0 would be invalid
	static ShaderID id = 1;
	return id++;
}
inline bool CheckCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
				<< "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog 
				<<  "\n -- --------------------------------------------------- -- " << std::endl;
			return false;
		}
	}
	return true;
}
inline void CompileShader(std::string& filename, GLuint shaderType,
						  std::string shaderTypeName, unsigned int& shader)
{
	std::string code;
	std::ifstream fileStream;
	fileStream.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		fileStream.open(filename);
		std::stringstream ss;
		ss << fileStream.rdbuf();
		fileStream.close();
		code = ss.str();
	}
	catch (std::ifstream::failure e)
	{
		const std::string& v(filename);
		auto start = v.find_last_of('/') + 1;
		auto len = v.find_last_of('.') - start;
		std::cout << "ERROR :: SHADER " << v.substr(start, len)
			<< " :: FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char * cCode = code.c_str();
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &cCode, nullptr);
	glCompileShader(shader);
	CheckCompileErrors(shader, shaderTypeName);
}
ShaderID LRendererGL3::CreateShader(std::string filename, bool vert, 
									bool frag, bool geom, bool tess, bool comp)
{
	std::lock_guard<std::mutex> guard(shader_mutex);
	// change this default path with a var in settings
	const auto path = "Data/Shaders/";
	std::array<std::string, SHADER_LEN> paths;
	if (vert) paths[0] = path + filename + ".vert";
	if (frag) paths[1] = path + filename + ".frag";
	if (geom) paths[2] = path + filename + ".geom";
	if (tess) paths[3] = path + filename + ".tesc";
	if (tess) paths[4] = path + filename + ".tese";
	if (comp) paths[5] = path + filename + ".comp";

	Shader_GL3 newShader;
	unsigned int shaderCode[SHADER_LEN];

	const GLuint shaderType[SHADER_LEN]{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_GEOMETRY_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_TESS_EVALUATION_SHADER,
		GL_COMPUTE_SHADER
	};

	const std::string shaderName[SHADER_LEN]{
		"VERTEX",
		"FRAGMENT",
		"GEOMETRY",
		"TESS CONTROL",
		"TESS EVALUATION",
		"COMPUTE"
	};

	for (int i = 0; i < SHADER_LEN; ++i)
	{
		if (!paths[i].empty())
			CompileShader(paths[i], shaderType[i], shaderName[i], shaderCode[i]);
	}

	newShader._shaderID = glCreateProgram();
	for (int i = 0; i < SHADER_LEN; ++i)
	{
		if (!paths[i].empty()) glAttachShader(newShader._shaderID, shaderCode[i]);
	}
	glLinkProgram(newShader._shaderID);
	if (!CheckCompileErrors(newShader._shaderID, "PROGRAM"))
	{
		return 0u;
	}

	for (int i = 0; i < SHADER_LEN; ++i)
	{
		if (!paths[i].empty()) glDeleteShader(shaderCode[i]);
	}

	newShader._initialized = true;

	const std::string& v(paths[0]);
	auto start = v.find_last_of('/') + 1;
	auto len = v.find_last_of('.') - start;

	std::cout << "Shader Loaded: " << v.substr(start, len) << std::endl;

	newShader._id = GenerateShaderID();
	shaders.emplace_back(newShader);

	GetUniforms(newShader._id);
	GetAttributes(newShader._id);

	return newShader._id;
}
void LRendererGL3::GetUniforms(ShaderID shaderID)
{
	auto& shader = shaders[shaderID];
	auto id = shader._shaderID;

	GLint count;
	glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &count);

	const GLsizei bufSize = 16;
	GLsizei length;
	GLint size;
	GLenum type;
	GLchar name[bufSize];

	for (GLint i = 0; i < count; ++i)
	{
		glGetActiveUniform(id, (GLuint) i, bufSize, &length, &size, &type, name);

		std::string n{ name };
		if (n.find('[') != std::string::npos)
		{
			n = n.substr(0, n.size() - 3);
		}

		GLint loc = glGetUniformLocation(id, name);

		if (!shader._uniforms.count(name))
		{
			shader._uniforms[name] = { loc, type };
		}
	}
}
void LRendererGL3::GetAttributes(ShaderID shaderID)
{
	auto& shader = shaders[shaderID];
	auto id = shader._shaderID;

	GLint count;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count);

	const GLsizei bufSize = 16;
	GLsizei length;
	GLint size;
	GLenum type;
	GLchar name[bufSize];

	for (GLint i = 0; i < count; ++i)
	{
		glGetActiveAttrib(id, (GLuint) i, bufSize, &length, &size, &type, name);
		std::string n{ name };
		if (n.find('[') != std::string::npos)
		{
			n = n.substr(0, n.size() - 3);
		}

		GLint loc = glGetUniformLocation(id, name);

		if (!shader._attributes.count(name))
		{
			shader._attributes[name] = { loc, type };
		}
	}
}
void LRendererGL3::SetActiveProgram(ShaderID shaderID)
{
	if (HasShader(shaderID))
	{
		LOG("No such shader using shaderID %u\n", shaderID);
		return;
	}
	const auto& shader = shaders[shaderID];
	if (!shader._initialized)
	{
		LOG("Shader not initialized\n");
		return;
	}
	glUseProgram(shader._shaderID);
}
void LRendererGL3::SetUniform(ShaderID shaderID, const std::string& name, 
									  std::any val)
{
	auto shader = shaders[shaderID];
	if (!shader._uniforms.count(name))
	{
		LOG("%s parameter not found\n", name.c_str());
		return;
	}
	const auto& param = shader._uniforms[name];

	// we use switch here so theres the least amount of overhead as possible
	switch (param.second)
	{
	case GL_BOOL:
		glUniform1i(param.first, (int)std::any_cast<bool>(val));
		return;
	case GL_INT:
		glUniform1i(param.first, std::any_cast<int>(val));
		return;
	case GL_FLOAT:
		glUniform1f(param.first, std::any_cast<float>(val));
		return;
	case GL_FLOAT_VEC2:
		glUniform2fv(param.first, 1, &std::any_cast<glm::vec2>(val)[0]);
		return;
	case GL_FLOAT_VEC3:
		glUniform3fv(param.first, 1, &std::any_cast<glm::vec3>(val)[0]);
		return;
	case GL_FLOAT_VEC4:
		glUniform4fv(param.first, 1, &std::any_cast<glm::vec4>(val)[0]);
		return;
	case GL_FLOAT_MAT2:
		glUniformMatrix2fv(param.first, 1, GL_FALSE, &std::any_cast<glm::mat2>(val)[0][0]);
		return;
	case GL_FLOAT_MAT3:
		glUniformMatrix3fv(param.first, 1, GL_FALSE, &std::any_cast<glm::mat3>(val)[0][0]);
		return;
	case GL_FLOAT_MAT4:
		glUniformMatrix4fv(param.first, 1, GL_FALSE, &std::any_cast<glm::mat4>(val)[0][0]);
		return;
	case GL_SAMPLER_2D:
		glUniform1i(param.first, std::any_cast<int>(val));
		return;
	default:
		LOG("%s's type not supported\n", name.c_str());
		return;
	}
}
bool LRendererGL3::HasShader(ShaderID shaderID)
{
	return std::any_of(shaders.begin(), shaders.end(), [=] (const Shader_GL3& s) 
	{
		return s._id == shaderID;
	});
}

// Buffer
inline BufferID GenerateBufferID()
{
	// ID = 0 would be invalid
	static BufferID id = 1;
	return id++;
}
inline void CheckFrameBufferStatus(std::string bufferName)
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR :: " << bufferName << " :: Framebuffer is not complete!" << std::endl;
}
inline void InitFramebuffer(Buffer_GL3& buffer)
{
	glGenFramebuffers(1, &buffer.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
inline void InitColorbuffer(Buffer_GL3& buffer)
{
	glGenTextures(1, &buffer.colorbuffer);
	glBindTexture(buffer.textureType, buffer.colorbuffer);
	glBindTexture(buffer.textureType, 0);
}
inline void InitDepthbuffer(Buffer_GL3& buffer)
{
	glGenRenderbuffers(1, &buffer.depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.depthbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
BufferID LRendererGL3::CreateBuffer(int MSAALevel, bool frame, bool color, bool depth)
{
	std::lock_guard<std::mutex> guard(buffer_mutex);
	//LOG("Locked buffer\n");
	Buffer_GL3 newBuffer;
	newBuffer.MSAALevel = MSAALevel;
	newBuffer.textureType = (MSAALevel == 1) ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
	
	if (frame) InitFramebuffer(newBuffer);
	if (color) InitColorbuffer(newBuffer);
	if (depth) InitDepthbuffer(newBuffer);
	auto newID = GenerateBufferID();
	buffers.emplace_back(newBuffer);

	return newID;
}
void LRendererGL3::BindColorbuffer(BufferID bufferID)
{
	std::string bufferName;
	Buffer_GL3& buffer = buffers[bufferID];

	glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
	glBindTexture(buffer.textureType, buffer.colorbuffer);

	if (buffer.textureType == GL_TEXTURE_2D)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 
					 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_2D, buffer.colorbuffer, 0);
		bufferName = "TEXTUREBUFFER";
	}
	else if (buffer.textureType == GL_TEXTURE_2D_MULTISAMPLE)
	{
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, buffer.MSAALevel,
								GL_RGB, size.x, size.y, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							   GL_TEXTURE_2D_MULTISAMPLE, buffer.colorbuffer, 0);
		bufferName = "MSAA TEXTUREBUFFER";
	}
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	CheckFrameBufferStatus(bufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void LRendererGL3::BindDepthbuffer(BufferID bufferID)
{
	Buffer_GL3& buffer = buffers[bufferID];

	glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.depthbuffer);
	if (buffer.MSAALevel == 1)
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
	}
	else
	{
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, buffer.MSAALevel, GL_DEPTH24_STENCIL8,
										 size.x, size.y);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
							  GL_RENDERBUFFER, buffer.depthbuffer);
	CheckFrameBufferStatus("DEPTHBUFFER");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void LRendererGL3::BindFramebuffer(BufferID bufferID)
{
	Buffer_GL3& buffer = buffers[bufferID];
	glViewport(0, 0, size.x, size.y);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
	// Remember to clear the color
// 	// Move this to the camera
// 	glClearColor(col.r, col.g, col.b, col.a);
// 	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void LRendererGL3::CopyBuffer(BufferID readID, BufferID drawID)
{
	Buffer_GL3& msaa = buffers[readID];
	Buffer_GL3& draw = buffers[drawID];
	glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa.framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw.framebuffer);
	glBlitFramebuffer(0, 0, size.x, size.y, 0, 0, size.x, size.y,
					  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
void LRendererGL3::UnbindFramebuffer(BufferID bufferID)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Mesh
inline MeshID GenerateMeshID()
{
	// ID = 0 would be invalid
	static MeshID id = 1;
	return id++;
}
inline void CreateVAO(Mesh_GL3& newMesh)
{
	glGenVertexArrays(1, &newMesh.VAO);
	glBindVertexArray(newMesh.VAO);
	glBindVertexArray(0);
}
inline void CreateVBO(GLuint dataSize, Mesh_GL3& newMesh)
{
	glGenBuffers(1, &newMesh.VBO);
	glBindVertexArray(newMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
	glBufferData(GL_ARRAY_BUFFER, newMesh.vertices.size() * sizeof(Vertex), 
				 &newMesh.vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
// This is for adding instance data
// - here we use our current vao but supply a new vbo to hold our 
// instance data
inline void CreateVBO(GLuint& vao, GLuint& vbo,
					  GLuint64 size, void * data, GLuint drawType)
{
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
inline void CreateEBO(Mesh_GL3& newMesh)
{
	glGenBuffers(1, &newMesh.EBO);
	glBindVertexArray(newMesh.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.indices.size() * sizeof(GLuint),
				 &newMesh.indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
inline void AddAttribute(int attribute, int dataSize,
						 GLuint dataLength, GLuint64 offset, Mesh_GL3& newMesh)
{
	glBindVertexArray(newMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, dataLength, (GLvoid*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
inline void AddAttribute(GLuint& vao, GLuint& vbo, int attribute, int dataSize,
						 GLuint dataLength, GLuint64 offset)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, dataLength, (GLvoid*) offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
inline void AddInstancedAttribute(int attribute, int dataSize,
						GLuint dataLength, GLuint64 offset, Mesh_GL3& newMesh)
{
	glBindVertexArray(newMesh.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, dataLength, (GLvoid*) offset);
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
inline void AddInstancedAttribute(GLuint& vao, GLuint& vbo, int attribute,
						int dataSize, GLuint dataLength, GLuint64 offset)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(attribute);
	glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE, dataLength, (GLvoid*) offset);
	glVertexAttribDivisor(attribute, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
inline void InitBuffer(Mesh_GL3& newMesh)
{
	// Create and bind vertex and index data
	newMesh.VAO = 0;
	newMesh.VBO = 0;
	newMesh.EBO = 0;

	GLuint size = sizeof(Vertex);
	GLuint instanceSize = sizeof(glm::mat4);
	CreateVAO(newMesh);
	CreateVBO(size, newMesh);
	CreateEBO(newMesh);

	AddAttribute(0, 3, size, offsetof(Vertex, Vertex::Position), newMesh);
	AddAttribute(1, 3, size, offsetof(Vertex, Vertex::Normal), newMesh);
	AddAttribute(2, 2, size, offsetof(Vertex, Vertex::TexCoords), newMesh);
	AddAttribute(3, 3, size, offsetof(Vertex, Vertex::Tangent), newMesh);
	AddAttribute(4, 3, size, offsetof(Vertex, Vertex::Bitangent), newMesh);

	glm::mat4 matrices[MaxBatchSize<glm::mat4>()];
	CreateVBO(newMesh.VAO, newMesh.iVBO, MaxBatchSize<glm::mat4>() * instanceSize,
			  &matrices[0], GL_DYNAMIC_DRAW);

	AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 5, 4, instanceSize, 0);
	AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 6, 4, instanceSize, sizeof(glm::vec4));
	AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 7, 4, instanceSize, sizeof(glm::vec4) * 2);
	AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 8, 4, instanceSize, sizeof(glm::vec4) * 3);
}
inline void ProcessMesh(Mesh_GL3& newMesh, aiMesh * mesh)
{
	newMesh.vertices.reserve(mesh->mNumVertices);
	for (GLuint i = 0; i < mesh->mNumVertices; ++i)
	{
		newMesh.vertices.emplace_back(Vertex());
		auto& vertex = newMesh.vertices.back();

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
			newMesh.indices.emplace_back(face.mIndices[j]);
	}
}
inline void ProcessNode(Mesh_GL3& newMesh, aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh * mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(newMesh, mesh);
	}
	for (GLuint i = 0; i < node->mNumChildren; ++i)
	{
		ProcessNode(newMesh, node->mChildren[i], scene);
	}
}
MeshID LRendererGL3::LoadModel(std::string filename)
{
	std::lock_guard<std::mutex> guard(mesh_mutex);
	Assimp::Importer importer;
	const aiScene * scene =
		importer.ReadFile(MESH_FOLDER + filename, aiProcessPreset_TargetRealtime_MaxQuality);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) return 0u;

	Mesh_GL3 newMesh;
	ProcessNode(newMesh, scene->mRootNode, scene);

	InitBuffer(newMesh);

	MeshID newID = GenerateMeshID();
	meshes[newID] = newMesh;

	return newID;
}
void LRendererGL3::DrawModel(MeshID meshID)
{
	auto& mesh = meshes[meshID];
	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glDrawElements(GL_TRIANGLES, (GLuint) mesh.indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void LRendererGL3::DrawModelInstanced(MeshID meshID, int size)
{
	auto& mesh = meshes[meshID];
	glBindVertexArray(mesh.VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
	glDrawElementsInstanced(GL_TRIANGLES, (GLuint) mesh.indices.size(),
							GL_UNSIGNED_INT, nullptr, size);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// Texture
inline TextureID GenerateTextureID()
{
	// ID = 0 would be invalid
	static TextureID id = 1;
	return id++;
}
TextureID LRendererGL3::LoadTexture2D(std::string filename)
{
	std::lock_guard<std::mutex> guard(texture_mutex);
	Texture_GL3 newTexture;

	std::string file{ TEXTURE_FOLDER };
	file.append(filename);
	auto * data = stbi_load(file.c_str(), &newTexture.width, 
							&newTexture.height, &newTexture.nrChannels, 0);
	GLenum format;
	if (newTexture.nrChannels == 1) { format = GL_RED; }
	else if (newTexture.nrChannels == 3) { format = GL_RGB; }
	else if (newTexture.nrChannels == 4) { format = GL_RGBA; }

	if (data)
	{
		glGenTextures(1, &newTexture.textureID);
		glBindTexture(GL_TEXTURE_2D, newTexture.textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, newTexture.width,
					 newTexture.height, 0, format, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
						format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
						format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		const std::string& f(filename);
		LOG("Texture loaded: %s", f.substr(f.find_last_of('/') + 1));
	}
	else
	{
		LOG("Texture not loaded: %s", filename.c_str());
		return 0u;
	}
	stbi_image_free(data);

	TextureID newTexID = GenerateTextureID();
	newTexture.textureType = GL_TEXTURE_2D;
	textures[newTexID] = newTexture;

	return newTexID;
}
void LRendererGL3::SetTexture(int idx, TextureID textureID)
{
	auto& tex = textures[textureID];
	glActiveTexture(GL_TEXTURE0 + idx);
	glBindTexture(tex.textureType, tex.textureID);
}