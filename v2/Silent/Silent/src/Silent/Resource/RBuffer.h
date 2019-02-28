#pragma once

#include "../Core.h"
#include "Resource.h"

#include "../Renderer/LRenderer.h"

namespace Silent
{
	class SILENT_API RBuffer : public Resource
	{
	protected:
		LRenderer * _renderer;
		BufferID _bufferID;
		int _MSAALevel = 1;

		bool useFrame;
		bool useColor;
		bool useDepth;

		void BindColorbuffer();
		void BindDepthbuffer();

	public:
		RBuffer(std::string name = "", LRenderer * renderer = nullptr, int msaa = 1,
				bool frame = true, bool color= true, bool depth = true);

		int GetMultisampleLevel() const;
		BufferID GetBufferID() const;

		void Begin();
		void End();

		inline void Load() override;
		inline void Cleanup() override;


	};
}