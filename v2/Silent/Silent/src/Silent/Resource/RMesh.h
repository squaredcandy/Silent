#pragma once

#include "../Core.h"
#include "Resource.h"

#include "../Renderer/LRenderer.h"

namespace Silent
{
	class SILENT_API RMesh : public Resource
	{
	protected:
		MeshID _meshID;
		LRenderer * _renderer = nullptr;
	public:
		RMesh(std::string name = "", LRenderer * renderer = nullptr);
		virtual ~RMesh() = default;

		void Cleanup() override;
		
		
		MeshID GetMeshID() const;
	};
}