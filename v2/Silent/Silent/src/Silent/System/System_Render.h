#pragma once
#include "../Core.h"
#include "System.h"

#include <ImGui/imgui.h>

namespace Silent
{
	struct ModelStructure
	{
		Module_Transform * transform;
		Module_Model * model;
		Module_Render * render;


	};

	class SILENT_API System_Render : public System
	{
		std::vector<ModelStructure> _models;

		MapTypeToConAModule _modules;
		Module_Camera * _camera;
		
		inline glm::vec3 GetFaceCameraVector();
		inline glm::mat4 GetProjectionMatrix(ImVec2& size);
		inline glm::mat4 GetViewMatrix();
		inline glm::mat4 GetModelMatrix(const Module_Transform * tf);

	public:
		System_Render(SystemPriority priority = 1000) : System(priority){}
		virtual ~System_Render() = default;

		virtual void UpdateEntities(Modules& modules) override;
		virtual void Execute() override;
	};
}