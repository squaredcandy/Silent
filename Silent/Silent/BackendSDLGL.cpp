#include "BackendSDLGL.h"

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

BackendSDLGL::BackendSDLGL()
{
}

BackendSDLGL::~BackendSDLGL()
{
}

void BackendSDLGL::Init()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL Error: %s\n", SDL_GetError());
	}

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
	//SDL_DisplayMode current;
	//SDL_GetCurrentDisplayMode(0, &current);
	
	using namespace Silent::Settings;
	window = SDL_CreateWindow(programName.c_str(),
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  windowWidth, windowHeight,
							  SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | 
							  SDL_WINDOW_ALLOW_HIGHDPI);
	context = SDL_GL_CreateContext(window);

	SDL_GL_SetSwapInterval(vSync); // Enable vsync
	gl3wInit();

	// Setup ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init(/*"#version 410 core"*/);
}

void BackendSDLGL::SettingsInit()
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
}

bool BackendSDLGL::UpdateEvent()
{
	bool ret = false;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			ret = true;
	}
	return ret;
}

void BackendSDLGL::Update()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window);
	ImGui::NewFrame();
}

void BackendSDLGL::Clear()
{
	glViewport(0, 0, 
		(int) ImGui::GetIO().DisplaySize.x, 
		(int) ImGui::GetIO().DisplaySize.y
	);
	using namespace Silent::Settings;
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void BackendSDLGL::Render()
{
	ImGui::Render();
	SDL_GL_MakeCurrent(window, context);
	Clear();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	SDL_GL_MakeCurrent(window, context);
	SDL_GL_SwapWindow(window);
}

void BackendSDLGL::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void BackendSDLGL::SetVSync(bool on)
{
	SDL_GL_SetSwapInterval(on); // Enable vsync
}

void BackendSDLGL::ResizeWindow(int w, int h)
{
	SDL_SetWindowSize(window, w, h);
}
