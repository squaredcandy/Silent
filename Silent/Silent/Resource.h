#pragma once

#define DATA_FOLDER "Data/"
#define MESH_FOLDER DATA_FOLDER "Meshes/"
#define TEXTURE_FOLDER DATA_FOLDER "Textures/"

class Resource
{
public:
	Resource() = default;
	virtual ~Resource() = default;

	virtual void Cleanup() = 0;
};