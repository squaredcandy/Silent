#pragma once

#include "../Core.h"
#include "Resource.h"
#include "../Renderer/LRenderer.h"

namespace Silent
{
	class SILENT_API RTexture : public Resource
	{
	protected:
		TextureID _textureID = 0;
	public:
		RTexture(std::string name = "");
		RTexture(std::string name, LRenderer * renderer);
		virtual ~RTexture() = default;

		void Cleanup() override;
		TextureID GetTextureID() const;
	};
}