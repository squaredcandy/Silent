#pragma once
#include "../Core.h"
#include "../Layer/Layer.h"
#include "../Event/EventDispatcher.h"
#include <any>

namespace Silent
{
	using ShaderID = unsigned int;
	using BufferID = unsigned int;
	using MeshID = unsigned int;
	using TextureID = unsigned int;
	using MaterialID = unsigned int;

	// Apprently the optimal batch size is about 1-4MB
	const int BATCH_SIZE = 500000;

	template<typename T>
	constexpr int MaxBatchSize()
	{
		return BATCH_SIZE / sizeof(T);
	}

	class SILENT_API LRenderer : public Layer
	{
	private:
		void AddCallback();
	protected:
		glm::ivec2 size;
	public:
		const unsigned int _rendererType;
		LRenderer(unsigned int rendererType = 0) : Layer(), 
			_rendererType(rendererType) 
		{
			AddCallback();
		}
		~LRenderer() = default;

		virtual void Clear(glm::vec4 col) = 0;

		// Shader
		virtual ShaderID CreateShader(std::string filename, bool vert = true, bool frag = true, 
									  bool geom = false, bool tess = false, bool comp = false) = 0;
		virtual void GetUniforms(ShaderID shaderID) = 0;
		virtual void GetAttributes(ShaderID shaderID) = 0;
		virtual void SetActiveProgram(ShaderID shaderID) = 0;
		virtual void SetUniform(ShaderID shaderID, const std::string& name, std::any val) = 0;

		// Buffer
		virtual BufferID CreateBuffer(int MSAALevel = true, bool frame = true, bool color = true, 
									  bool depth = true) = 0;
		virtual void BindColorbuffer(BufferID bufferID) = 0;
		virtual void BindDepthbuffer(BufferID bufferID) = 0;
		virtual void BindFramebuffer(BufferID bufferID) = 0;
		virtual void CopyBuffer(BufferID readID, BufferID drawID) = 0;
		virtual void UnbindFramebuffer(BufferID bufferID) = 0;

		// Mesh
		virtual MeshID LoadModel(std::string filename) = 0;
		virtual void DrawModel(MeshID meshID) = 0;
		virtual void DrawModelInstanced(MeshID meshID, int size) = 0;

		// Texture
		virtual TextureID LoadTexture2D(std::string filename) = 0;
		virtual void SetTexture(int idx, TextureID textureID) = 0;
	};
}