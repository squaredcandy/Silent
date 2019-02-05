#pragma once

#include "../Core.h"
#include "Resource.h"

#include "../Renderer/Renderer.h"

namespace Silent
{
	class SILENT_API RMesh : public Resource
	{
	protected:
		MeshID _meshID;
		Renderer * _renderer;
	public:
		RMesh(std::string name = "");
		RMesh(std::string name, Renderer * renderer);
		virtual ~RMesh() = default;

		void Cleanup() override;
		
		
		MeshID GetMeshID() const;
	};
}