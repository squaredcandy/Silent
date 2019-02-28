#pragma once
#include "../Core.h"
#include "LRenderer.h"
#include <GL/gl3w.h>

namespace Silent
{
	class SILENT_API Shader_GL3
	{
	public:
		bool _initialized;
		ShaderID _id;
		GLuint _shaderID;
		// param name, < location, type >
		std::map<std::string, std::pair<GLint, GLenum>> _uniforms;
		std::map<std::string, std::pair<GLint, GLenum>> _attributes;
	};
	class SILENT_API Buffer_GL3
	{
	public:
		GLuint framebuffer;
		GLuint colorbuffer;
		GLuint depthbuffer;

		// You could argue that we can save some memory and calculate it 
		// everytime saving it is much easier and allows us to pass as const
		int MSAALevel;
		GLuint textureType;
	};
	struct SILENT_API Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;

		bool operator==(const Vertex& other) const
		{
			return Position == other.Position && Normal == other.Normal &&
				TexCoords == other.TexCoords && Tangent == other.Tangent &&
				Bitangent == other.Bitangent;
		}
	};
	class SILENT_API Mesh_GL3
	{
	public:
		// Vertex Array Object
		// Vertex Buffer Object
		// Element Buffer Object
		// Instance Vertex Buffer Object
		GLuint VAO, VBO, EBO, iVBO;

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
	};
	class SILENT_API Texture_GL3
	{
	public:
		GLuint textureID;
		int width;
		int height;
		int nrChannels;
		GLenum textureType;
	};

	const int SHADER_LEN = 6;

	class SILENT_API LRendererGL3 : public LRenderer
	{
	protected:
		// Storage
		std::vector<Shader_GL3> shaders;
		std::vector<Buffer_GL3> buffers;
		std::vector<Mesh_GL3> meshes;
		std::vector<Texture_GL3> textures;
	public:
		LRendererGL3();
		virtual ~LRendererGL3() = default;

		inline void OnInitialize() override;
		inline void OnUpdate() override;
		inline void OnDestroy() override;
		
		inline void Clear(glm::vec4 col = { 0, 0, 0, 0 }) override;

		// Shaders
		inline bool HasShader(ShaderID shaderID);
		inline ShaderID CreateShader(std::string filename, bool vert = true, bool frag = true, 
								bool geom = false, bool tess = false, bool comp = false) override;
		inline void GetUniforms(ShaderID shaderID) override;
		inline void GetAttributes(ShaderID shaderID) override;
		inline void SetActiveProgram(ShaderID shaderID) override;
		inline void SetUniform(ShaderID shaderID, const std::string& name, std::any val) override;

		// Buffers
		inline BufferID CreateBuffer(int MSAALevel = true, bool frame = true, bool color = true,
									 bool depth = true) override;
		inline void BindColorbuffer(BufferID bufferID) override;
		inline void BindDepthbuffer(BufferID bufferID) override;
		inline void BindFramebuffer(BufferID bufferID) override;
		inline void CopyBuffer(BufferID readID, BufferID drawID) override;
		inline void UnbindFramebuffer(BufferID bufferID) override;

		// Mesh
		inline MeshID LoadModel(std::string filename) override;
		inline void DrawModel(MeshID meshID) override;
		inline void DrawModelInstanced(MeshID meshID, int size) override;

		// Texture
		inline TextureID LoadTexture2D(std::string filename) override;
		inline void SetTexture(int idx, TextureID textureID) override;
	};
}