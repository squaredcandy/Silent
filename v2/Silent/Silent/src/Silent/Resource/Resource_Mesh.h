#pragma once

#include "../Core.h"
#include "Resource.h"

#include "../Renderer/Renderer.h"

namespace Silent
{
	class SILENT_API Resource_Mesh : public Resource
	{
	protected:
		MeshID _meshID;
	public:
		Resource_Mesh(std::string name = "");
		Resource_Mesh(std::string name, Renderer * renderer);
		virtual ~Resource_Mesh() = default;

		void Cleanup() override;
		
		MeshID GetMeshID() const;
	};
}