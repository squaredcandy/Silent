#include "RMaterial.h"

namespace Silent
{
	MaterialID GenerateMaterialID()
	{
		static MaterialID newID = 1u;
		return newID++;
	}

	RMaterial::RMaterial(std::string name) : Resource(name), _materialID(GenerateMaterialID())
	{

	}

	void RMaterial::SetShader(std::shared_ptr<RShader> shader)
	{
		_shader = shader;
		// We clear any uniforms cos it may not match up with the new shader
		// we are using
		uniformParams.clear();
	}

	void RMaterial::AddTexture(int idx, std::shared_ptr<RTexture> texture)
	{
		_textures[idx] = texture;
	}

	void RMaterial::AddParameter(const std::string& name, const std::any& val)
	{
		uniformParams[name] = val;
	}

	MaterialID RMaterial::GetMaterialID() const
	{
		return _materialID;
	}

	void RMaterial::Cleanup()
	{
		//_shader.reset();
		_textures.clear();
		uniformParams.clear();
	}

}