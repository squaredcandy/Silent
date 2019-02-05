#pragma once

#include "../Core.h"
#include "System.h"

#include <ImGui/imgui.h>

namespace Silent
{
	struct CameraStructure
	{
		std::shared_ptr<MCamera> _camera;
		std::shared_ptr<MTransform> _camtf;

		CameraStructure() : _camera(nullptr), _camtf(nullptr) {}
		CameraStructure(std::shared_ptr<MCamera> camera, 
						std::shared_ptr<MTransform> tf)
			: _camera(camera), _camtf(tf) {}
		~CameraStructure() = default;
	};

	class SILENT_API SCamera : public System
	{
	protected:
		CameraStructure _cam;

		void UpdateCamera();
	public:
		float translationSpeed = 5.f;
		float rotationSpeed = 10.f;

		SCamera(SystemPriority priority = 2) : System("Camera", priority) {}
		virtual ~SCamera() = default;

		virtual void Execute() override;
		virtual void Cleanup() override;

		inline void CameraTranslation(float dt);
		inline void CameraRotation(float dt);

		CameraStructure& GetCameras() { return _cam; }

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;


		virtual void DebugInfo() override;

	};
}