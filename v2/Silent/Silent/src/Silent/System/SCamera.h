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
		//CameraStructure(MCamera * camera, MTransform * tf)
		//	: _camera(camera), _camtf(tf) {}
		CameraStructure(std::shared_ptr<MCamera> camera, 
						std::shared_ptr<MTransform> tf)
			: _camera(camera), _camtf(tf) {}
		~CameraStructure() = default;

		glm::vec3& Translation() { return _camtf->_translate; }
		glm::vec3& Rotation() { return _camtf->_rotate; }
		glm::vec3& Scale() { return _camtf->_scale; }

		glm::vec3& ForwardVector() { return _camtf->_forwardVector; }
		glm::vec3& RightVector() { return _camtf->_rightVector; }
		glm::vec3& UpVector() { return _camtf->_upVector; }
	};

	class SILENT_API SCamera : public System
	{
	protected:
		CameraStructure _cam;

		void UpdateCamera();
	public:

		SCamera(SystemPriority priority = 2) : System("Camera", priority) {}
		virtual ~SCamera() = default;

		virtual void Execute() override;
		virtual void Cleanup() override;

		inline void CameraTranslation(float dt);
		inline void CameraRotation(float dt);
		inline void UpdateCameraVectors();

		inline glm::vec3 GetFaceCameraVector();
		inline glm::mat4 GetProjectionMatrix(ImVec2& size);
		inline glm::mat4 GetViewMatrix();

		CameraStructure& GetCameras() { return _cam; }

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;


		virtual void DebugInfo() override;

	};
}