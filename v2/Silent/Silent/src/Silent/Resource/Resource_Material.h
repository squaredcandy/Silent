#pragma once

#include "../Core.h"
#include "Resource.h"
#include "Resource_Shader.h"
#include "Resource_Texture.h"

namespace Silent
{
	class SILENT_API Resource_Material : public Resource
	{
	protected:
		MaterialID _materialID;

	public:
		// here we set a pointer to the shader we are going to use
		std::shared_ptr<Resource_Shader> _shader;
		// we use a map int - tex so we can skip texture numbers if we want
		// and more easily override existing textures
		std::map<int, std::shared_ptr<Resource_Texture>> _textures;

		// Here we store an instance of all the uniform parameters 
		// we want to modify
		std::map<std::string, std::any> uniformParams;

		Resource_Material(std::string name = "");
		virtual ~Resource_Material() = default;

		void SetShader(std::shared_ptr<Resource_Shader> shader);
		void AddTexture(int idx, std::shared_ptr<Resource_Texture> texture);
		void AddParameter(const std::string& name, const std::any& val);

		MaterialID GetMaterialID() const;

		void Cleanup() override;
	};
}