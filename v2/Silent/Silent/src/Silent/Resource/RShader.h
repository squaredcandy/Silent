#pragma once

#include "../Core.h"
#include "Resource.h"
#include "../Renderer/Renderer.h"

namespace Silent
{
	class SILENT_API RShader : public Resource
	{
	protected:
		bool _initialized;
		ShaderID _shaderID;

		Renderer * _renderer;
	public:
		RShader(std::string name = "");
		RShader(std::string name, Renderer * renderer,
						bool useVert = true, bool useFrag = true,
						bool useGeom = false, bool useTess = false,
						bool useComp = false);
		virtual ~RShader() = default;
		
		inline void SetActive();
		inline void SetUniform(const std::string& name, const std::any& val);

		ShaderID GetShaderID() const;
		Renderer * GetRenderer() const;
		
		void Cleanup() override;
	};
}