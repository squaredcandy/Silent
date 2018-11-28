#pragma once

#include "Backend.h"

#include <string>
#include <ImGui/imgui.h>
#include <memory>

#define MIN_WIDTH 800
#define MIN_HEIGHT 600

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
}