#pragma once

#include "Renderer.h"

#include <GL/gl3w.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#define RGB_BUFFER_SIZE 8
#define DEPTH_BUFFER_SIZE 24
#define STENCIL_BUFFER_SIZE 8

#define OPENGL_MAJOR_VER 4
#define OPENGL_MINOR_VER 3

namespace Silent
{
	class SILENT_API Shader_GL3
	{
	public:
		bool _initialized;
		GLuint _shaderID;
		std::map<std::string, std::pair<GLint, GLenum>> _parameters;
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
			return Position == other.Position &&
				   Normal == other.Normal &&
				   TexCoords == other.TexCoords &&
				   Tangent == other.Tangent &&
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

	class SILENT_API Renderer_GL3 : public Renderer
	{
	private:
		SDL_GLContext context;

		// Shaders/Meshes/Textures + anything gl reliant are all held in here
		// This makes it easy to swap out the functionality if we use vulkan		
		// Shader Code
		std::map<ShaderID, Shader_GL3> shaders;
		bool CheckCompileErrors(GLuint shader, std::string type);
		void CompileShader(std::string & filename, GLuint shaderType, 
						   std::string shaderTypeName, unsigned int & shader);
		ShaderID InitShader(std::array<std::string, SHADER_LEN> paths);

		// Buffer Code
		std::map<BufferID, Buffer_GL3> buffers;
		void InitFramebuffer(Buffer_GL3& buffer);
		void InitColorbuffer(Buffer_GL3& buffer);
		void InitDepthbuffer(Buffer_GL3& buffer);

		// Mesh Code
		std::map<MeshID, Mesh_GL3> meshes;
		void InitBuffer(Mesh_GL3& newMesh);
		void CreateVAO(Mesh_GL3& newMesh);
		void CreateVBO(GLuint dataSize, Mesh_GL3& newMesh);
		void CreateEBO(Mesh_GL3& newMesh);

		void CreateVBO(GLuint& vao, GLuint& vbo, GLuint64 size,
					   void * data, GLuint drawType);
		void AddAttribute(int attribute, int dataSize,
						  GLuint dataLength, GLuint64 offset, Mesh_GL3& newMesh);

		void AddAttribute(GLuint& vao, GLuint& vbo, int attribute, int dataSize,
						  GLuint dataLength, GLuint64 offset);
		void AddInstancedAttribute(int attribute, int dataSize,
								   GLuint dataLength, GLuint64 offset, 
								   Mesh_GL3& newMesh);
		void AddInstancedAttribute(GLuint& vao, GLuint& vbo, int attribute,
								   int dataSize, GLuint dataLength, GLuint64 offset);

		// Texture Code
		std::map<TextureID, Texture_GL3> textures;

	public:
		Renderer_GL3() = default;
		virtual ~Renderer_GL3() = default;

		// Init
		inline int PreWindowInitialization() override;
		inline int GetWindowFlags() override;
		inline int PostWindowInitialization(SDL_Window * window) override;
		inline int ImGuiInitialization(SDL_Window * window) override;
		inline int SettingsInitialization() override;

		// New Frame
		inline int NewFrame() override;
		inline int PreClear(SDL_Window * window) override;
		inline int Clear() override;
		inline int PostClear() override;
		inline int Flip(SDL_Window * window) override;
		inline int Cleanup(SDL_Window * window) override;

		// Shader
		inline ShaderID CreateShader(std::string filename, bool useVert = true,
									 bool useFrag = true, bool useGeom = false,
									 bool useTess = false, 
									 bool useComp = false) override;
		inline void GetAttributes(ShaderID shaderID) override;
		inline void GetUniforms(ShaderID shaderID) override;
		inline void SetActiveProgram(ShaderID shaderID) override;
		inline void SetUniform(ShaderID shaderID, const std::string& name,
							   std::any val) override;

		// Buffer
		inline BufferID CreateBuffer(int MSAALevel = 1, 
									 bool useFrame = true, bool useColor = true,  
									 bool useDepth = true) override;
		inline void BindColorbufferToFramebuffer(BufferID bufferID) override;
		inline void BindDepthbufferToFramebuffer(BufferID bufferID) override;
		inline void BeginFramebufferDrawing(BufferID bufferID) override;
		inline void CopyMSAAbufferToDrawbuffer(BufferID msaaID, 
											   BufferID drawID) override;
		inline void EndFramebufferDrawing(BufferID bufferID, 
										  float sizeX, float sizeY,
										  float posX, float posY) override;
		// Mesh
		inline MeshID LoadModel(std::string filename) override;
		// This will need to be updated somehow
		inline void MapModelData(MeshID meshID, const std::vector<glm::mat4>& data) override;
		inline void DrawModel(MeshID meshID) override;
		inline void DrawModelInstanced(MeshID meshID, int size) override;

		// Texture
		inline TextureID LoadTexture2D(std::string filename) override;
		inline void SetTexture(int idx, TextureID textureID) override;
	};
}