#include "Settings.h"

#include "Log.h"
#include <algorithm>

namespace Silent::Settings
{
	std::string programName = "Silent Engine";
	int windowWidth = 1600;
	int windowHeight = 900;
	bool vSync = true;
	bool showLog = true;
	ImVec4 infoColor[5] =
	{
		ImColor(0.2f, 0.8f, 0.2f),
		ImColor(1.0f, 1.0f, 1.0f),
		ImColor(1.0f, 0.8f, 0.0f),
		ImColor(0.9f, 0.0f, 0.0f),
		ImColor(0.0f, 0.8f, 0.8f)
	};
	ImVec4 clearColor = ImColor(114, 144, 154);

	void SettingsGUI()
	{
		if (ImGui::InputInt("Window Width", &windowWidth, 160, 160))
		{
			//LOG_INFO("Changed Window Width");
			windowWidth = std::max(windowWidth, MIN_WIDTH);
			Engine::backend->ResizeWindow(windowWidth, windowHeight);
		}
		if (ImGui::InputInt("Window Height", &windowHeight, 90, 90))
		{
			//LOG_INFO("Change Window Height");
			windowHeight = std::max(windowHeight, MIN_HEIGHT);
			Engine::backend->ResizeWindow(windowWidth, windowHeight);
		}
		if (ImGui::Checkbox("vSync", &vSync))
		{
			LOG_INFO("Turned vSync %s", (vSync) ? "On" : "Off");
			Engine::backend->SetVSync(vSync);
		}
	}
}