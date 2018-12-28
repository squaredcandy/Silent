#include "Resource_Texture.h"

namespace Silent
{

	Resource_Texture::Resource_Texture(std::string name) : Resource()
	{

	}

	Resource_Texture::Resource_Texture(std::string name, Renderer * renderer)
		: Resource()
	{
		_textureID = renderer->LoadTexture2D(name);
		if (_textureID != 0) _initialized = true;
	}

	void Resource_Texture::Cleanup()
	{

	}

	TextureID Resource_Texture::GetTextureID() const
	{
		return _textureID;
	}

}