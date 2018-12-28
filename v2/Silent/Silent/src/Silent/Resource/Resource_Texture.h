#pragma once

#include "../Core.h"
#include "Resource.h"
#include "../Renderer/Renderer.h"

namespace Silent
{
	class SILENT_API Resource_Texture : public Resource
	{
	protected:
		TextureID _textureID;
	public:
		Resource_Texture(std::string name = "");
		Resource_Texture(std::string name, Renderer * renderer);

		void Cleanup() override;
		TextureID GetTextureID() const;
	};
}