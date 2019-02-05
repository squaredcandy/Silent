#include "RShader.h"

#include <glm/matrix.hpp>

namespace Silent
{
	RShader::RShader(std::string name)
		: Resource(), _shaderID(0u) {}

	RShader::RShader(std::string name, Renderer * renderer,
									 bool useVert, bool useFrag,
									 bool useGeom, bool useTess,
									 bool useComp) : Resource()
	{
		_renderer = renderer;
		_shaderID = _renderer->CreateShader(name, useVert, useFrag,
										   useGeom, useTess, useComp);
		if(_shaderID != 0u) _initialized = true;
		
		_renderer->SetActiveProgram(_shaderID);
		_renderer->GetUniforms(_shaderID);
	}

	void RShader::SetActive()
	{
		_renderer->SetActiveProgram(_shaderID);
	}

	void RShader::SetUniform(const std::string& name, 
									 const std::any& val)
	{
		_renderer->SetUniform(_shaderID, name, val);
	}

	Silent::ShaderID RShader::GetShaderID() const
	{
		return _shaderID;
	}

	void RShader::Cleanup()
	{

	}

	Silent::Renderer * RShader::GetRenderer() const
	{ 
		return _renderer; 
	}

}