#include "Resource_Material.h"

namespace Silent
{
	MaterialID GenerateMaterialID()
	{
		static MaterialID newID = 1u;
		return newID++;
	}

	Resource_Material::Resource_Material(std::string name) 
		: _materialID(GenerateMaterialID())
	{

	}

	void Resource_Material::SetShader(std::shared_ptr<Resource_Shader> shader)
	{
		_shader = shader;
		// We clear any uniforms cos it may not match up with the new shader
		// we are using
		uniformParams.clear();
	}

	void Resource_Material::AddTexture(int idx, 
									   std::shared_ptr<Resource_Texture> texture)
	{
		_textures[idx] = texture;
	}

	void Resource_Material::AddParameter(const std::string& name, 
										 const std::any& val)
	{
		uniformParams[name] = val;
	}

	MaterialID Resource_Material::GetMaterialID() const
	{
		return _materialID;
	}

	void Resource_Material::Cleanup()
	{
		//_shader.reset();
		_textures.clear();
		uniformParams.clear();
	}

}