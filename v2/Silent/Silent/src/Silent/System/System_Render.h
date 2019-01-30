#pragma once
#include "../Core.h"
#include "System.h"
#include "System_Light.h"
#include "System_Camera.h"

namespace Silent
{
	class SILENT_API Resource_Buffer;
	class SILENT_API Resource_Mesh;
	class SILENT_API Resource_Material;
	struct ModelStructure
	{
		std::shared_ptr<Resource_Buffer> buffer;
		std::shared_ptr<Resource_Mesh> mesh;
		std::shared_ptr<Resource_Material> material;

		bool operator==(const ModelStructure& other) const;
		bool operator<(const ModelStructure& other) const;
	};

	struct ModelMatrixStructure
	{
		std::vector<Module_Transform *> tf;
		std::vector<glm::mat4> model;
	};


	class SILENT_API System_Render : public System
	{
		std::map<ModelStructure, ModelMatrixStructure> _models;
		inline glm::mat4 GetModelMatrix(const Module_Transform * tf);

		void UpdateModelMatrix();

	public:
		System_Light * _lightSystem;
		System_Camera * _cameraSystem;

		System_Render(SystemPriority priority = 1000) : System("Renderer", priority) {}
		virtual ~System_Render() = default;

		virtual void Execute() override;
		virtual void Cleanup() override;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;

		virtual void DebugInfo() override;

	};
}