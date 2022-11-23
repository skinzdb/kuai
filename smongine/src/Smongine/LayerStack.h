#pragma once

#include <deque>

#include "Smongine/Core.h"
#include "Layer.h"

namespace Smong {
	class SMONGINE_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::deque<Layer*>::iterator begin() { return layers.begin(); }
		std::deque<Layer*>::iterator end() { return layers.end(); }


	private:
		std::deque<Layer*> layers;
		std::deque<Layer*>::iterator layerInsert;
	};
}


