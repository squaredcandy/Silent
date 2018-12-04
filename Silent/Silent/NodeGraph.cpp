#include "NodeGraph.h"

#include "Nodes.h"

namespace Silent::Engine::NodeGraph
{
	std::vector<std::unique_ptr<Node>> nodes;

	void ExampleNodes()
	{
		nodes.emplace_back(std::move(std::make_unique<NodeAddFloat>(ImVec2(500, 100))));
	}

}