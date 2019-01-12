#include "Resource_Mesh.h"


namespace Silent
{
	Resource_Mesh::Resource_Mesh(std::string name) : Resource()
	{
		
	}

	Resource_Mesh::Resource_Mesh(std::string name, Renderer * renderer)
		: Resource(), _renderer(renderer)
	{
		_meshID = _renderer->LoadModel(name);
	}

	void Resource_Mesh::Cleanup()
	{

	}

	MeshID Resource_Mesh::GetMeshID() const
	{
		return _meshID;
	}

}