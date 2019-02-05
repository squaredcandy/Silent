#include "RMesh.h"

namespace Silent
{
	RMesh::RMesh(std::string name) : Resource()
	{
		
	}

	RMesh::RMesh(std::string name, Renderer * renderer)
		: Resource(), _renderer(renderer)
	{
		_meshID = _renderer->LoadModel(name);
	}

	void RMesh::Cleanup()
	{

	}

	MeshID RMesh::GetMeshID() const
	{
		return _meshID;
	}

}