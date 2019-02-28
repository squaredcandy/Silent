#include "RMesh.h"

namespace Silent
{
	RMesh::RMesh(std::string name, LRenderer * renderer) : Resource(name), _renderer(renderer)
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