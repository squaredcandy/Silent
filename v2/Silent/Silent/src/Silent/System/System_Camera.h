#pragma once

#include "../Core.h"
#include "System.h"

namespace Silent
{
	class SILENT_API System_Camera : public System
	{
		MapTypeToConAModule _modules;
		Module_Camera * _camera;
	public:

		System_Camera(SystemPriority priority = 2) : System(priority){}
		virtual ~System_Camera() = default;

		virtual void UpdateEntities(Modules& modules) override;
		virtual void Execute() override;

		void CameraTranslation(float dt);
		void CameraRotation(float dt);
		void UpdateCameraVectors();
	};
}