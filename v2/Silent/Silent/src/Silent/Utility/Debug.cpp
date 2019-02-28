#include "Debug.h"

#include <ImGui/imgui.h>
#include <numeric>

using namespace Silent;

void LDebug::ShowLog()
{
	auto fold = [] (std::string a, std::string b)
	{
		a += b + "\n";
		return a;
	};
	bool a = true;

	static bool ScrollToBottom = true;

	if (ImGui::Begin("Debug Log", &a, ImGuiWindowFlags_MenuBar))
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("Scroll To Bottom", "", &ScrollToBottom);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		for (const auto& l : log->log)
		{
			ImGui::Text("%s", l.c_str());
		}
		if(ScrollToBottom) ImGui::SetScrollHereY(0.999f);
	}
	ImGui::End();
}

void Silent::LDebug::OnInitialize()
{
	log = Locator<LLog>::Get();
}

void Silent::LDebug::OnUpdate()
{
	ShowLog();
}

void Silent::LDebug::OnDestroy()
{
}
