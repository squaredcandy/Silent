#pragma once

#include "Entities.h"

namespace Silent::Engine::Widget
{
	extern EntityID selectedID;

	void MainDockspace(bool * open);
	void ObjectInspector(bool * open);
	void Stats(bool * open);
	void ConfigureSettings(bool * open);
	void DrawLog();
	void DrawNodeGraphProperties(bool * open);
	void DrawNodeGraph(bool * open);
}