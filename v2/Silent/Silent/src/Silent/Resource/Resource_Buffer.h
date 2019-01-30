#pragma once

#include "../Core.h"
#include "Resource.h"

#include "../Renderer/Renderer.h"

namespace Silent
{
	class SILENT_API Resource_Buffer : public Resource
	{
	protected:
		Renderer * _renderer;
		BufferID _bufferID;
		int _MSAALevel = 1;

		bool useFrame;
		bool useColor;
		bool useDepth;

		void BindColorbuffer();
		void BindDepthbuffer();

	public:
		Resource_Buffer(std::string name = "");
		Resource_Buffer(std::string name, Renderer * renderer,
						bool useFramebuffer = true,  bool useColorbuffer = true, 
						bool useDepthbuffer = true);

		int GetMultisampleLevel() const;
		BufferID GetBufferID() const;

		void Begin();
		void End();

		void Cleanup() override;
	};
}