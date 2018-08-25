#pragma once

#include <string>
#include <ImGui/imgui.h>

namespace Silent::Settings
{
	extern std::string programName;
	extern int windowWidth;
	extern int windowHeight;
	extern bool vSync;

	extern ImVec4 clearColor;
}