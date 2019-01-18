#pragma once
#include "../Core.h"
#include "System.h"

#include <ImGui/imgui.h>
#include <tuple>

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

	class SILENT_API System_Render : public System
	{
		std::map<ModelStructure, std::vector<glm::mat4>> _models;

		MapTypeToConAModule _modules;
		Module_Camera * _camera;
		
		inline glm::vec3 GetFaceCameraVector();
		inline glm::mat4 GetProjectionMatrix(ImVec2& size);
		inline glm::mat4 GetViewMatrix();
		inline glm::mat4 GetModelMatrix(const Module_Transform * tf);

	public:
		System_Render(SystemPriority priority = 1000) : System(priority){}
		virtual ~System_Render() = default;

		virtual void Execute() override;
		virtual void Cleanup() override;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;

	};
}