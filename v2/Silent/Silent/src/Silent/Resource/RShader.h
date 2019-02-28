#pragma once

#include "../Core.h"
#include "Resource.h"
#include "../Renderer/LRenderer.h"

namespace Silent
{
	class SILENT_API RShader : public Resource
	{
	protected:
		bool _initialized = false;
		ShaderID _shaderID;

		LRenderer * _renderer;
	public:
		RShader(std::string name = "");
		RShader(std::string name, LRenderer * renderer,
						bool useVert = true, bool useFrag = true,
						bool useGeom = false, bool useTess = false,
						bool useComp = false);
		virtual ~RShader() = default;
		
		inline void SetActive();
		inline void SetUniform(const std::string& name, const std::any& val);

		ShaderID GetShaderID() const;
		LRenderer * GetRenderer() const;
		
		void Cleanup() override;
	};
}