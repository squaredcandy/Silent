#pragma once
#include "../Core.h"
#include "System.h"
#include "SLight.h"
#include "SCamera.h"

namespace Silent
{
	class SILENT_API RBuffer;
	class SILENT_API RMesh;
	class SILENT_API RMaterial;
	struct ModelStructure
	{
		std::shared_ptr<RBuffer> buffer;
		std::shared_ptr<RMesh> mesh;
		std::shared_ptr<RMaterial> material;

		bool operator==(const ModelStructure& other) const;
		bool operator<(const ModelStructure& other) const;
	};

	struct ModelMatrixStructure
	{
		std::vector<std::shared_ptr<MTransform>> tf;
		std::vector<glm::mat4> model;
	};


	class SILENT_API SRender : public System
	{
		std::map<ModelStructure, ModelMatrixStructure> _models;

		void UpdateModels();
		void UpdateModelMatrix();

	public:
		SLight * _lightSystem;
		SCamera * _cameraSystem;

		SRender(SystemPriority priority = 1000) : 
			System("Renderer", priority) {}
		virtual ~SRender() = default;

		virtual void Execute() override;
		virtual void Cleanup() override;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;

		virtual void DebugInfo() override;

	};
}