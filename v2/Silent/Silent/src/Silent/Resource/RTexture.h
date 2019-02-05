#pragma once

#include "../Core.h"
#include "Resource.h"
#include "../Renderer/Renderer.h"

namespace Silent
{
	class SILENT_API RTexture : public Resource
	{
	protected:
		TextureID _textureID;
	public:
		RTexture(std::string name = "");
		RTexture(std::string name, Renderer * renderer);
		virtual ~RTexture() = default;

		void Cleanup() override;
		TextureID GetTextureID() const;
	};
}