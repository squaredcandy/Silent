#pragma once

#include "Node.h"

namespace Silent::Engine::NodeGraph
{
	extern std::vector<std::unique_ptr<Node>> nodes;

	void ExampleNodes();
}