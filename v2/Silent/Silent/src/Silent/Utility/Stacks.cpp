#include "Stacks.h"

using namespace Silent;
using vLayer = std::vector<Layer*>;
inline vLayer::iterator EraseLayer(vLayer& stack, vLayer::iterator& it, bool remove)
{
	(*it)->OnDestroy();
	if(remove) delete *it;
	//*it = nullptr;
	return stack.erase(it);
}
Stacks::Stacks()
{

}
Stacks::~Stacks()
{
	for (auto it = mainStack.end(); it != mainStack.begin();)
	{
		it--;
		it = EraseLayer(mainStack, it, true);
	}
	for (auto it = overlayStack.end(); it != overlayStack.begin();)
	{
		it--;
		it = EraseLayer(overlayStack, it, true);
	}
}
void Stacks::PushMainLayer(Layer * layer)
{
	mainStack.emplace_back(layer);
	layer->OnInitialize();
}
void Stacks::PopMainLayer(Layer * layer, bool remove)
{
	auto it = std::find(mainStack.begin(), mainStack.end(), layer);
	if (it != mainStack.end())
	{
		EraseLayer(mainStack, it, remove);
	}
}
void Stacks::PushOverlayLayer(Layer * layer)
{
	overlayStack.emplace_back(layer);
	layer->OnInitialize();
}
void Stacks::PopOverlayLayer(Layer * layer, bool remove)
{
	auto it = std::find(overlayStack.begin(), overlayStack.end(), layer);
	if (it != overlayStack.end())
	{
		EraseLayer(overlayStack, it, remove);
	}
}
void Stacks::UpdateStacks()
{
	for (auto& layer : mainStack)
	{
		layer->OnUpdate();
	}
	for (auto& layer : overlayStack)
	{
		layer->OnUpdate();
	}
}