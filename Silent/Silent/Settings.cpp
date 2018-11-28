#include "Settings.h"

#include "Log.h"
#include <algorithm>

#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <iomanip>

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

	std::array<float, 4> FromImVec4(ImVec4 v)
	{
		return { v.x, v.y, v.z, v.w };
	}

	ImVec4 ToImVec4(std::array<float, 4> a)
	{
		return { a[0], a[1], a[2], a[3] };
	}


	void ChangeWindowWidth()
	{
		windowWidth = std::max(windowWidth, MIN_WIDTH);
		Engine::backend->ResizeWindow(windowWidth, windowHeight);
	}

	void ChangeWindowHeight()
	{
		windowHeight = std::max(windowHeight, MIN_HEIGHT);
		Engine::backend->ResizeWindow(windowWidth, windowHeight);
	}

	void ChangeVSync()
	{
		LOG_INFO("Turned vSync %s", (vSync) ? "On" : "Off");
		Engine::backend->SetVSync(vSync);
	}

	void SaveSettings()
	{
		nlohmann::json saveFile;

		saveFile[HEIGHT_NAME] = windowHeight;
		saveFile[WIDTH_NAME] = windowWidth;
		saveFile[VSYNC_NAME] = vSync;
		saveFile[SHOW_LOG_NAME] = showLog;

		saveFile[TRACE_COL_NAME] = FromImVec4(infoColor[0]);
		saveFile[INFO_COL_NAME] = FromImVec4(infoColor[1]);
		saveFile[WARN_COL_NAME] = FromImVec4(infoColor[2]);
		saveFile[ERROR_COL_NAME] = FromImVec4(infoColor[3]);
		saveFile[FATAL_COL_NAME] = FromImVec4(infoColor[4]);

		std::ofstream o(SAVE_LOCATION, std::ofstream::out);
		o << std::setw(4) << saveFile << std::endl;
		o.close();
		LOG_INFO("Saved File to %s", SAVE_LOCATION);
	}

	void LoadSettings()
	{
		nlohmann::json loadFile;
		std::ifstream i(SAVE_LOCATION, std::ifstream::in);
		i >> loadFile;

		windowHeight = loadFile[HEIGHT_NAME];
		windowWidth = loadFile[WIDTH_NAME];
		vSync = loadFile[VSYNC_NAME];
		showLog = loadFile[SHOW_LOG_NAME];

		infoColor[0] = ToImVec4(loadFile[TRACE_COL_NAME]);
		infoColor[1] = ToImVec4(loadFile[INFO_COL_NAME]);
		infoColor[2] = ToImVec4(loadFile[WARN_COL_NAME]);
		infoColor[3] = ToImVec4(loadFile[ERROR_COL_NAME]);
		infoColor[4] = ToImVec4(loadFile[FATAL_COL_NAME]);

		ChangeWindowWidth();
		ChangeWindowHeight();
		ChangeVSync();

		i.close();
		LOG_INFO("Loaded File from %s", SAVE_LOCATION);
	}


	void SettingsGUI()
	{
		if (ImGui::Button("Save Settings"))
		{
			SaveSettings();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Settings"))
		{
			LoadSettings();
		}
		if (ImGui::InputInt("Window Width", &windowWidth, 160, 160))
		{
			ChangeWindowWidth();
		}
		if (ImGui::InputInt("Window Height", &windowHeight, 90, 90))
		{
			ChangeWindowHeight();
		}
		if (ImGui::Checkbox("vSync", &vSync))
		{
			ChangeVSync();
		}
	}
}