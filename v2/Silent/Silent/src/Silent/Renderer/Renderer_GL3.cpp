#include "Renderer_GL3.h"

//#include <GL/gl3w.h>
#include <GL/gl3w.c>
#include <SDL/SDL.h>
//#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader.h>

#include <sstream>

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

void CheckFrameBufferStatus(std::string bufferName)
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR :: " << bufferName <<
		" :: Framebuffer is not complete!" << std::endl;
}

namespace Silent
{
	void ChangeColors()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.76f, 0.76f, 0.76f, 0.40f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.99f, 0.99f, 0.99f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.74f, 0.74f, 0.74f, 0.31f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.80f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.69f, 0.20f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.76f, 0.40f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.34f, 0.34f, 0.34f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.23f, 0.23f, 0.23f, 0.95f);
		colors[ImGuiCol_Tab] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
		colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.65f, 0.10f, 0.80f);
		colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.89f, 0.55f, 0.00f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	int Renderer_GL3::PreWindowInitialization()
	{
		// Setup window
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
							| SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
							SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, RGB_BUFFER_SIZE);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, RGB_BUFFER_SIZE);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, RGB_BUFFER_SIZE);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, RGB_BUFFER_SIZE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUFFER_SIZE);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_BUFFER_SIZE);

		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VER);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VER);
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::GetWindowFlags()
	{
		return SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE |
			SDL_WINDOW_ALLOW_HIGHDPI;
	}

	int Renderer_GL3::PostWindowInitialization(SDL_Window * window)
	{
		context = SDL_GL_CreateContext(window);
		SDL_GL_SetSwapInterval(0); // Enable vsync
		gl3wInit();
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::ImGuiInitialization(SDL_Window * window)
	{
		ImGui_ImplSDL2_InitForOpenGL(window, context);
		ImGui_ImplOpenGL3_Init(/*"#version 410 core"*/);
		ChangeColors();
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::SettingsInitialization()
	{
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
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
							  GL_DONT_CARE, 0, nullptr, true);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
							  GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr,
							  false);
		glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER,
							  GL_DONT_CARE, 0, nullptr, false);
		// 	glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		//glPatchParameteri(GL_PATCH_VERTICES, 3);

		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::NewFrame()
	{
		windowSize = ImGui::GetIO().DisplaySize;
		ImGui_ImplOpenGL3_NewFrame();
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::PreClear(SDL_Window * window)
	{
		SDL_GL_MakeCurrent(window, context);
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::Clear()
	{
		glViewport(0, 0,
			(int) ImGui::GetIO().DisplaySize.x,
				   (int) ImGui::GetIO().DisplaySize.y
		);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::PostClear()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::Flip(SDL_Window * window)
	{
		SDL_GL_MakeCurrent(window, context);
		SDL_GL_SwapWindow(window);
		return SL_ERROR_NONE;
	}

	int Renderer_GL3::Cleanup(SDL_Window * window)
	{
		for (const auto&[key, val] : buffers)
		{
			glDeleteBuffers(1, &val.framebuffer);
			glDeleteBuffers(1, &val.colorbuffer);
			glDeleteBuffers(1, &val.depthbuffer);
		}

		// Destroy Shaders
		for (const auto& [key, val] : shaders)
		{
			glDeleteBuffers(1, &val._shaderID);
		}

		// Destroy Meshes
		for (const auto& [key, val] : meshes)
		{
			glDeleteVertexArrays(1, &val.VAO);
			glDeleteBuffers(1, &val.VBO);
			glDeleteBuffers(1, &val.EBO);
			glDeleteBuffers(1, &val.iVBO);
		}

		// Destroy Textures
		for (const auto&[key, val] : textures)
		{
			glDeleteTextures(1, &val.textureID);
		}

		buffers.clear();
		shaders.clear();
		meshes.clear();
		textures.clear();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return SL_ERROR_NONE;
	}
}

// Holds the buffer code, splitting it so we can categorize it more easily
namespace Silent
{
	inline BufferID GenerateBufferID()
	{
		// ID = 0 would be invalid
		static BufferID id = 1;
		return id++;
	}

	BufferID Renderer_GL3::CreateBuffer(int MSAALevel, bool useFrame, 
										bool useColor, bool useDepth)
	{
		Buffer_GL3 newBuffer;
		newBuffer.MSAALevel = MSAALevel;
		newBuffer.textureType = (MSAALevel == 1) ?
			GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;
		if (useFrame) InitFramebuffer(newBuffer);
		if (useColor) InitColorbuffer(newBuffer);
		if (useDepth) InitDepthbuffer(newBuffer);
		auto newID = GenerateBufferID();
		buffers[newID] = newBuffer;
		return newID;
	}

	void Renderer_GL3::BindColorbufferToFramebuffer(BufferID bufferID)
	{
		std::string bufferName;
		Buffer_GL3& buffer = buffers[bufferID];

		glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
		glBindTexture(buffer.textureType, buffer.colorbuffer);
		if (buffer.textureType == GL_TEXTURE_2D)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
						 (GLint) windowSize.x, (GLint) windowSize.y,
						 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								   GL_TEXTURE_2D, buffer.colorbuffer, 0);
			bufferName = "TEXTUREBUFFER";
		}
		else if (buffer.textureType == GL_TEXTURE_2D_MULTISAMPLE)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, buffer.MSAALevel, 
									GL_RGB, (GLint) windowSize.x, 
									(GLint) windowSize.y, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								   GL_TEXTURE_2D_MULTISAMPLE, 
								   buffer.colorbuffer, 0);
			bufferName = "MSAA TEXTUREBUFFER";
		}
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		CheckFrameBufferStatus(bufferName);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer_GL3::BindDepthbufferToFramebuffer(BufferID bufferID)
	{
		Buffer_GL3& buffer = buffers[bufferID];
		auto size = ImGui::GetIO().DisplaySize;

		glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, buffer.depthbuffer);
		if (buffer.MSAALevel == 1)
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
				(GLint) size.x, (GLint) size.y);
		}
		else
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, buffer.MSAALevel,
											 GL_DEPTH24_STENCIL8,
											 (GLint) windowSize.x, 
											 (GLint) windowSize.y);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
								  GL_RENDERBUFFER, buffer.depthbuffer);
		CheckFrameBufferStatus("DEPTHBUFFER");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	inline void Renderer_GL3::BeginFramebufferDrawing(BufferID bufferID)
	{
		Buffer_GL3& buffer = buffers[bufferID];
		glViewport(0, 0, (GLint) windowSize.x, (GLint) windowSize.y);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
		// Move this to the camera
		ImVec4 clear_col = ImColor(15, 94, 156);
		glClearColor(clear_col.x, clear_col.y, clear_col.z, clear_col.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer_GL3::CopyMSAAbufferToDrawbuffer(BufferID msaaID, BufferID drawID)
	{
		Buffer_GL3& msaa = buffers[msaaID];
		Buffer_GL3& draw = buffers[drawID];
		glBindFramebuffer(GL_READ_FRAMEBUFFER, msaa.framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw.framebuffer);
		glBlitFramebuffer(0, 0, (GLint) windowSize.x, (GLint) windowSize.y,
						  0, 0, (GLint) windowSize.x, (GLint) windowSize.y,
						  GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
						  GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	void Renderer_GL3::EndFramebufferDrawing(BufferID bufferID, 
											 float sizeX, float sizeY, 
											 float posX, float posY)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		Buffer_GL3& buffer = buffers[bufferID];
// 		if (buffer.MSAALevel != 1)
// 		{
// 		}

		glViewport(0, 0, (GLint) windowSize.x, (GLint) windowSize.y);

		glBindFramebuffer(GL_FRAMEBUFFER, (GLuint) buffer.framebuffer);

		ImVec2 start = { posX, posY };
		ImVec2 end = { posX + sizeX, posY + sizeY };

		ImGui::GetWindowDrawList()->AddImageRounded(
			reinterpret_cast<GLvoid*>((GLint64) buffer.framebuffer),
			start, end, { 0, 1 }, { 1, 0 }, 0xFFFFFFFF,
			ImGui::GetStyle().WindowRounding);
		glBindVertexArray(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Renderer_GL3::InitFramebuffer(Buffer_GL3& buffer)
	{
		glGenFramebuffers(1, &buffer.framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, buffer.framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Renderer_GL3::InitColorbuffer(Buffer_GL3& buffer)
	{
		glGenTextures(1, &buffer.colorbuffer);
		glBindTexture(buffer.textureType, buffer.colorbuffer);
		glBindTexture(buffer.textureType, 0);
	}

	void Renderer_GL3::InitDepthbuffer(Buffer_GL3& buffer)
	{
		glGenRenderbuffers(1, &buffer.depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, buffer.depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
}

#include <glm/gtc/type_ptr.hpp>

// Holds the shader code, spliting it up so we can categorize it more easily
namespace Silent
{
	inline ShaderID GenerateShaderID()
	{
		// ID = 0 would be invalid
		static ShaderID id = 1;
		return id++;
	}

	bool Renderer_GL3::CheckCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				return false;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				return false;
			}
		}
		return true;
	}

	void Renderer_GL3::CompileShader(std::string& filename, GLuint shaderType,
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

	ShaderID Renderer_GL3::InitShader(std::array<std::string, SHADER_LEN> paths)
	{
		Shader_GL3 newShader;
		unsigned int shaderCode[SHADER_LEN];

		const GLuint shaderType[SHADER_LEN] {
			GL_VERTEX_SHADER,
			GL_FRAGMENT_SHADER,
			GL_GEOMETRY_SHADER,
			GL_TESS_CONTROL_SHADER,
			GL_TESS_EVALUATION_SHADER,
			GL_COMPUTE_SHADER
		};

		const std::string shaderName[SHADER_LEN] {
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
				CompileShader(paths[i], shaderType[i],
							  shaderName[i], shaderCode[i]);
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

		ShaderID newShaderID = GenerateShaderID();
		shaders[newShaderID] = std::move(newShader);

		return newShaderID;
	}

	ShaderID Renderer_GL3::CreateShader(std::string filename,
		bool useVert, bool useFrag, bool useGeom, bool useTess, bool useComp)
	{
		// change this default path with a var in settings
		const auto path = "Data/Shaders/";
		std::array<std::string, SHADER_LEN> shaderPaths;
		if (useVert) shaderPaths[0] = path + filename + ".vert";
		if (useFrag) shaderPaths[1] = path + filename + ".frag";
		if (useGeom) shaderPaths[2] = path + filename + ".geom";
		if (useTess) shaderPaths[3] = path + filename + ".tesc";
		if (useTess) shaderPaths[4] = path + filename + ".tese";
		if (useComp) shaderPaths[5] = path + filename + ".comp";
		return InitShader(shaderPaths);
	}

	void Renderer_GL3::GetAttributes(ShaderID shaderID)
	{
		auto id = shaders[shaderID]._shaderID;

		GLint count;
		glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &count);

		printf("Active attributes %d", count);

		const GLsizei bufSize = 16;
		GLsizei length;
		GLint size;
		GLenum type;
		GLchar name[bufSize];


		for (GLint i = 0; i < count; ++i)
		{
			glGetActiveAttrib(id, (GLuint) i, bufSize, &length, &size, &type, name);
			printf("Attribute #%d type: %u Name: %s\n", i, type, name);
		}
	}

	void Renderer_GL3::GetUniforms(ShaderID shaderID)
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
			glGetActiveUniform(id, (GLuint) i, bufSize, &length, 
							   &size, &type, name);

			std::string n{ name };
			if (n.find('[') != std::string::npos)
			{
				n = n.substr(0, n.size() - 3);
			}

			GLint loc = glGetUniformLocation(id, name);

			if (!shader._parameters.count(name))
			{
				shader._parameters[name] = { loc, type };
			}
		}
	}

	void Renderer_GL3::SetActiveProgram(ShaderID shaderID)
	{
		if (!shaders.count(shaderID))
		{
			printf("No such shader using shaderID %u\n", shaderID);
			return;
		}
		const auto& shader = shaders[shaderID];
		if (!shader._initialized)
		{
			printf("Shader not initialized\n");
			return;
		}
		glUseProgram(shader._shaderID);
	}

	void Renderer_GL3::SetUniform(ShaderID shaderID, const std::string& name, std::any val)
	{
		auto shader = shaders[shaderID];
		if (!shader._parameters.count(name))
		{
			printf("%s parameter not found\n", name.c_str());
			return;
		}
		const auto& param = shader._parameters[name];

		// we use switch here so theres the least amount of overhead as possible
		switch (param.second)
		{
		case GL_BOOL:
			glUniform1i(param.first, 
						static_cast<int>(std::any_cast<bool>(val)));
			return;
		case GL_INT:
			glUniform1i(param.first, std::any_cast<int>(val));
			return;
		case GL_FLOAT:
			glUniform1f(param.first, std::any_cast<float>(val));
			return;
		case GL_FLOAT_VEC2:
			glUniform2fv(param.first, 1, 
						 glm::value_ptr(std::any_cast<glm::vec2>(val)));
			return;
		case GL_FLOAT_VEC3:
			glUniform3fv(param.first, 1, 
						 glm::value_ptr(std::any_cast<glm::vec3>(val)));
			return;
		case GL_FLOAT_VEC4:
			glUniform4fv(param.first, 1, 
						 glm::value_ptr(std::any_cast<glm::vec4>(val)));
			return;
		case GL_FLOAT_MAT2:
			glUniformMatrix2fv(param.first, 1, GL_FALSE, 
							   glm::value_ptr(std::any_cast<glm::mat2>(val)));
			return;
		case GL_FLOAT_MAT3:
			glUniformMatrix3fv(param.first, 1, GL_FALSE, 
							   glm::value_ptr(std::any_cast<glm::mat3>(val)));
			return;
		case GL_FLOAT_MAT4:
			glUniformMatrix4fv(param.first, 1, GL_FALSE, 
							   glm::value_ptr(std::any_cast<glm::mat4>(val)));
			return;
		case GL_SAMPLER_2D:
			glUniform1i(param.first, std::any_cast<int>(val));
			return;
		default:
			printf("%s's type not supported\n", name.c_str());
			return;
		}
	}
}

// This is a hash function for our vertex so we can check if vertices are 
// different
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
namespace std
{
	template<> struct hash<Silent::Vertex>
	{
		size_t operator()(Silent::Vertex const& vertex) const
		{
			std::size_t const h1 (std::hash<glm::vec3>()(vertex.Position));
			std::size_t const h2 (std::hash<glm::vec3>()(vertex.Normal));
			std::size_t const h3 (std::hash<glm::vec2>()(vertex.TexCoords));
			//std::size_t const h4 (std::hash<glm::vec3>()(vertex.Tangent));
			//std::size_t const h5 (std::hash<glm::vec3>()(vertex.Bitangent));

			return (((h1 ^ 
					(h2 << 1)) >> 1 ^ 
					(h3 << 1)) << 1);
		}
	};
}

// Holds the mesh code, splitting it up so we can categorize it more easily
namespace Silent
{
	inline MeshID GenerateMeshID()
	{
		// ID = 0 would be invalid
		static MeshID id = 1;
		return id++;
	}

	void Renderer_GL3::InitBuffer(Mesh_GL3& newMesh)
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

		glm::mat4 matrices[MaxBatchSize];
		CreateVBO(newMesh.VAO, newMesh.iVBO, MaxBatchSize * instanceSize,
				  &matrices[0], GL_DYNAMIC_DRAW);

		AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 5, 4, instanceSize, 0);
		AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 6, 4, instanceSize, sizeof(glm::vec4));
		AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 7, 4, instanceSize, sizeof(glm::vec4) * 2);
		AddInstancedAttribute(newMesh.VAO, newMesh.iVBO, 8, 4, instanceSize, sizeof(glm::vec4) * 3);
	}

	void Renderer_GL3::CreateVAO(Mesh_GL3& newMesh)
	{
		glGenVertexArrays(1, &newMesh.VAO);
		glBindVertexArray(newMesh.VAO);
		glBindVertexArray(0);
	}

	void Renderer_GL3::CreateVBO(GLuint dataSize, Mesh_GL3& newMesh)
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
	void Renderer_GL3::CreateVBO(GLuint& vao, GLuint& vbo,
						 GLuint64 size, void * data, GLuint drawType)
	{
		glGenBuffers(1, &vbo);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, size, data, drawType);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer_GL3::CreateEBO(Mesh_GL3& newMesh)
	{
		glGenBuffers(1, &newMesh.EBO);
		glBindVertexArray(newMesh.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newMesh.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.indices.size() * sizeof(GLuint),
					 &newMesh.indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer_GL3::AddAttribute(int attribute, int dataSize,
							GLuint dataLength, GLuint64 offset, Mesh_GL3& newMesh)
	{
		glBindVertexArray(newMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE,
							  dataLength, (GLvoid*) offset);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer_GL3::AddAttribute(GLuint& vao, GLuint& vbo, int attribute, int dataSize,
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

	void Renderer_GL3::AddInstancedAttribute(int attribute, int dataSize,
									 GLuint dataLength, GLuint64 offset, Mesh_GL3& newMesh)
	{
		glBindVertexArray(newMesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, newMesh.VBO);
		glEnableVertexAttribArray(attribute);
		glVertexAttribPointer(attribute, dataSize, GL_FLOAT, GL_FALSE,
							  dataLength, (GLvoid*) offset);
		glVertexAttribDivisor(attribute, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer_GL3::AddInstancedAttribute(GLuint& vao, GLuint& vbo, int attribute,
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

	MeshID Renderer_GL3::LoadModel(std::string path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		path.insert(0, MESH_FOLDER);
		std::string warn;
		std::string err;
		bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, 
									&warn, &err, path.c_str(), MESH_FOLDER);
		if (!err.empty()) std::cerr << err << std::endl;
		if (!ret) return 0u;

		Mesh_GL3 newMesh;
		newMesh.vertices.reserve(attrib.vertices.size());

		std::unordered_map<Vertex, GLuint> uniqueVerts;

		for (const auto & shape : shapes)
		{
			newMesh.indices.reserve(shape.mesh.indices.size());
			for (const auto& idx : shape.mesh.indices)
			{
				Vertex& newVertex = newMesh.vertices.emplace_back(Vertex());

				newVertex.Position = {
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				};

				newVertex.Normal = {
					attrib.normals[3 * idx.normal_index + 0],
					attrib.normals[3 * idx.normal_index + 1],
					attrib.normals[3 * idx.normal_index + 2]
				};

				newVertex.TexCoords = {
					attrib.texcoords[2 * idx.texcoord_index + 0],
					attrib.texcoords[2 * idx.texcoord_index + 1]
				};

				if (uniqueVerts.count(newVertex) == 0)
				{
					uniqueVerts[newVertex] = (GLuint)newMesh.indices.size();
				}

				newMesh.indices.emplace_back(uniqueVerts[newVertex]);
			}
		}

		//LOG_INFO("Model Loaded: %s\n", path.substr(path.find_last_of('/') + 1).c_str());

		InitBuffer(newMesh);

		MeshID newID = GenerateMeshID();
		meshes[newID] = newMesh;

		return newID;
	}
	
	void Renderer_GL3::MapModelData(MeshID meshID, const std::vector<glm::mat4>& data)
	{
		auto& mesh = meshes[meshID];
		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ARRAY_BUFFER, mesh.iVBO);
		auto size = sizeof(glm::mat4) * data.size();
		//glBufferSubData(GL_ARRAY_BUFFER, 0, size, &data[0]);

		void * ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, 
									  GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
		memcpy(ptr, data.data(), size);
		
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Renderer_GL3::DrawModel(MeshID meshID)
	{
		auto& mesh = meshes[meshID];
		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glDrawElements(GL_TRIANGLES, (GLuint) mesh.indices.size(),
					   GL_UNSIGNED_INT, nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	void Renderer_GL3::DrawModelInstanced(MeshID meshID, int size)
	{
		auto& mesh = meshes[meshID];
		glBindVertexArray(mesh.VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
		glDrawElementsInstanced(GL_TRIANGLES, (GLuint) mesh.indices.size(),
					   GL_UNSIGNED_INT, nullptr, size);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Holds the texture code, splitting it up so we categorize it more easily
namespace Silent
{
	inline TextureID GenerateTextureID()
	{
		// ID = 0 would be invalid
		static TextureID id = 1;
		return id++;
	}

	TextureID Renderer_GL3::LoadTexture2D(std::string filename)
	{
		Texture_GL3 newTexture;

		std::string file{ TEXTURE_FOLDER };
		file.append(filename);
		auto * data = stbi_load(file.c_str(), 
								&newTexture.width, 
								&newTexture.height, 
								&newTexture.nrChannels, 0);
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
			//LOG_INFO("Texture loaded: %s", f.substr(f.find_last_of('/') + 1));
		}
		else
		{
			//LOG_ERROR("Texture not loaded: %s", fileName);
			return 0u;
		}
		stbi_image_free(data);

		TextureID newTexID = GenerateTextureID();
		newTexture.textureType = GL_TEXTURE_2D;
		textures[newTexID] = newTexture;

		return newTexID;
	}

	void Renderer_GL3::SetTexture(int idx, TextureID textureID)
	{
		auto& tex = textures[textureID];
		glActiveTexture(GL_TEXTURE0 + idx);
		glBindTexture(tex.textureType, tex.textureID);
	}

}