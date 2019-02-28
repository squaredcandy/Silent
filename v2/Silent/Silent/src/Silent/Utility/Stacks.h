#pragma once

#include "../Core.h"
#include "../Layer/Layer.h"

namespace Silent
{
	class SILENT_API Stacks
	{
	private:
		using vLayer = std::vector<Layer*>;
		vLayer mainStack;
		vLayer overlayStack;
	public:
		Stacks();
		~Stacks();

		void UpdateStacks();

		void PushMainLayer(Layer * layer);
		void PopMainLayer(Layer * layer, bool remove = true);
		void PushOverlayLayer(Layer * layer);
		void PopOverlayLayer(Layer * layer, bool remove = true);
	};
}