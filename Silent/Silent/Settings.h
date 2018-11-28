#pragma once

#include "Backend.h"

#include <string>
#include <ImGui/imgui.h>
#include <memory>

#define SAVE_LOCATION "Save/Settings.json"

#define MIN_WIDTH 800
#define MIN_HEIGHT 600

#define HEIGHT_NAME "Window_Height"
#define WIDTH_NAME "Window_Width"
#define VSYNC_NAME "VSync"
#define SHOW_LOG_NAME "Show_Log"

#define TRACE_COL_NAME "Trace_Color"
#define INFO_COL_NAME "Info_Color"
#define WARN_COL_NAME "Warn_Color"
#define ERROR_COL_NAME "Error_Color"
#define FATAL_COL_NAME "Fatal_Color"

class Backend;

namespace Silent::Engine
{
	extern std::shared_ptr<Backend> backend;
}

namespace Silent::Settings
{
	extern std::string programName;
	extern int windowWidth;
	extern int windowHeight;
	extern bool vSync;

	extern bool showLog;

	extern ImVec4 infoColor[5];

	extern ImVec4 clearColor;

	void SettingsGUI();

	void SaveSettings();
}