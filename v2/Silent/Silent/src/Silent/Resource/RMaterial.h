#pragma once

#include "../Core.h"
#include "Resource.h"
#include "RShader.h"
#include "RTexture.h"

namespace Silent
{
	class SILENT_API RMaterial : public Resource
	{
	protected:
		MaterialID _materialID;

	public:
		// here we set a pointer to the shader we are going to use
		std::shared_ptr<RShader> _shader;
		// we use a map int - tex so we can skip texture numbers if we want
		// and more easily override existing textures
		std::map<int, std::shared_ptr<RTexture>> _textures;

		// Here we store an instance of all the uniform parameters 
		// we want to modify
		std::map<std::string, std::any> uniformParams;

		RMaterial(std::string name = "");
		virtual ~RMaterial() = default;

		void SetShader(std::shared_ptr<RShader> shader);
		void AddTexture(int idx, std::shared_ptr<RTexture> texture);
		void AddParameter(const std::string& name, const std::any& val);

		MaterialID GetMaterialID() const;

		void Cleanup() override;
	};
}