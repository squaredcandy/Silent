#include "RTexture.h"

namespace Silent
{

	RTexture::RTexture(std::string name) : Resource()
	{

	}

	RTexture::RTexture(std::string name, Renderer * renderer)
		: Resource()
	{
		_textureID = renderer->LoadTexture2D(name);
		if (_textureID != 0) _initialized = true;
	}

	void RTexture::Cleanup()
	{

	}

	TextureID RTexture::GetTextureID() const
	{
		return _textureID;
	}

}