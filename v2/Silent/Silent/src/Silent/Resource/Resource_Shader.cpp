#include "Resource_Shader.h"

#include <glm/matrix.hpp>

namespace Silent
{
	Resource_Shader::Resource_Shader(std::string name)
		: Resource(), _shaderID(0u) {}

	Resource_Shader::Resource_Shader(std::string name, Renderer * renderer,
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

	void Resource_Shader::SetActive()
	{
		_renderer->SetActiveProgram(_shaderID);
	}

	void Resource_Shader::SetUniform(const std::string& name, 
									 const std::any& val)
	{
		_renderer->SetUniform(_shaderID, name, val);
	}

	Silent::ShaderID Resource_Shader::GetShaderID() const
	{
		return _shaderID;
	}

	void Resource_Shader::Cleanup()
	{

	}

	Silent::Renderer * Resource_Shader::GetRenderer() const
	{ 
		return _renderer; 
	}

}