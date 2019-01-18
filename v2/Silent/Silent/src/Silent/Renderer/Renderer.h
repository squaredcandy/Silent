#pragma once

#include "../Core.h"

#include <SDL/SDL.h>
#include <ImGui/imgui.h>

#include <glm/glm.hpp>

#include <map>
#include <any>

namespace Silent
{
	using ShaderID = unsigned int;
	using BufferID = unsigned int;
	using MeshID = unsigned int;
	using TextureID = unsigned int;
	using MaterialID = unsigned int;

	// Apprently the optimal batch size is about 1-4MB
	// Make a function that does 1,000,000 / sizeof(Type) to give a batch size
	//const int MaxBatchSize = 1024;

	const int BATCH_SIZE = 1000000;

	template<typename T> 
	constexpr int MaxBatchSize()
	{
		return BATCH_SIZE / sizeof(T);
	}

	class SILENT_API Renderer
	{
	protected:
		// Save the window size so we only have to do it once per frame
		ImVec2 windowSize;
	public:
		virtual ~Renderer() = default;

		// Initialization
		virtual inline int PreWindowInitialization() = 0;
		virtual inline int GetWindowFlags() = 0;
		virtual inline int PostWindowInitialization(SDL_Window * window) = 0;
		virtual inline int ImGuiInitialization(SDL_Window * window) = 0;
		virtual inline int SettingsInitialization() = 0;

		// New Frame
		virtual inline int NewFrame() = 0;

		// Render
		virtual inline int PreClear(SDL_Window * window) = 0;
		virtual inline int Clear() = 0;
		virtual inline int PostClear() = 0;
		virtual inline int Flip(SDL_Window * window) = 0;
		virtual inline int Cleanup(SDL_Window * window) = 0;

		// Shader
		virtual inline ShaderID CreateShader(
			std::string filename, bool useVert = true, bool useFrag = true, 
			bool useGeom = true, bool useTess = true, bool useComp = true) = 0;
		virtual inline void GetAttributes(ShaderID shaderID) = 0;
		virtual inline void GetUniforms(ShaderID shaderID) = 0;
		virtual inline void SetActiveProgram(ShaderID shaderID) = 0;
		virtual inline void SetUniform(ShaderID shaderID, 
									   const std::string& name, 
									   std::any val) = 0;

		// Buffer
		virtual inline BufferID CreateBuffer(int MSAALevel = 1,
											 bool useFrame = true,
											 bool useColor = true, 
											 bool useDepth = true) = 0;
		virtual inline void
			BindColorbufferToFramebuffer(BufferID bufferID) = 0;
		virtual inline void 
			BindDepthbufferToFramebuffer(BufferID bufferID) = 0;
		virtual inline void
			BeginFramebufferDrawing(BufferID bufferID) = 0;
		virtual inline void
			CopyMSAAbufferToDrawbuffer(BufferID msaaID, BufferID drawID) = 0;
		virtual inline void
			EndFramebufferDrawing(BufferID bufferID, float sizeX, float sizeY,
								  float posX, float posY) = 0;

		// Mesh
		virtual inline MeshID LoadModel(std::string filename) = 0;
		virtual inline void MapModelData(MeshID meshID, const std::vector<glm::mat4>& data) = 0;
		virtual inline void DrawModel(MeshID meshID) = 0;
		virtual inline void DrawModelInstanced(MeshID meshID, int size) = 0;

		// Texture
		virtual inline TextureID LoadTexture2D(std::string filename) = 0;
		virtual inline void SetTexture(int idx, TextureID textureID) = 0;
	};

	// To be defined in renderers.cpp
	Renderer * GetRenderer(int RendererType);
}