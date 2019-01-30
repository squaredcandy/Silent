#include "System_Camera.h"
#include <SDL/SDL.h>

namespace Silent
{
	void System_Camera::Execute()
	{
		// Should have a better check for if we have a current camera or not
		if (!_cam._camera || !_cam._camtf) return;

		float dt = ImGui::GetIO().DeltaTime;

		CameraTranslation(dt);
		CameraRotation(dt);
		UpdateCameraVectors();
	}

	void System_Camera::Cleanup()
	{
		
	}

	glm::vec3 System_Camera::GetFaceCameraVector()
	{
		return glm::vec3(0, _cam.Rotation().x + 90.f, 0);
	}

	glm::mat4 System_Camera::GetProjectionMatrix(ImVec2& size)
	{
		return glm::perspective(_cam._camera->fov, size.x / size.y,
							_cam._camera->nearPlane, _cam._camera->farPlane);
	}

	glm::mat4 System_Camera::GetViewMatrix()
	{
		return glm::lookAt(_cam.Translation(),
						   _cam.Translation() + _cam.ForwardVector(),
						   _cam.UpVector());
	}

	void System_Camera::CameraTranslation(float dt)
	{
		float moveSpeed = _cam._camera->translateSpeed * dt;
		if (ImGui::IsKeyDown(SDL_SCANCODE_W))
			_cam.Translation() += _cam.ForwardVector() * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_S))
			_cam.Translation() -= _cam.ForwardVector() * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_D))
			_cam.Translation() += _cam.RightVector() * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_A))
			_cam.Translation() -= _cam.RightVector() * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_Q))
			_cam.Translation() += _cam.UpVector() * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_E))
			_cam.Translation() -= _cam.UpVector() * moveSpeed;
	}

	void System_Camera::CameraRotation(float dt)
	{
		if (!ImGui::IsMouseDown(1)) return;
		ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
		mouseDelta.x *= 0.01f;
		mouseDelta.y *= 0.01f;

		const float MaxLookHeight = 89.9f;
		
		float rotSpeed = _cam._camera->rotateSpeed;
		_cam.Rotation()[0] += mouseDelta.x * rotSpeed;
		_cam.Rotation()[1] = glm::clamp(_cam.Rotation()[1] -
			(mouseDelta.y * rotSpeed), -MaxLookHeight, MaxLookHeight);
	}

	void System_Camera::UpdateCameraVectors()
	{
		auto yawRad = glm::radians(_cam.Rotation()[0]);
		auto pitchRad = glm::radians(_cam.Rotation()[1]);
		auto cYawRad = cosf(yawRad);
		auto cPitchRad = cosf(pitchRad);
		auto sYawRad = sinf(yawRad);
		auto sPitchRad = sinf(pitchRad);

		const glm::vec3 worldUpVector{ 0.f, 1.f, 0.f };

		auto forward = glm::vec3(cYawRad * cPitchRad, sPitchRad, 
								 sYawRad * cPitchRad);
		_cam.ForwardVector() = glm::normalize(forward);
		_cam.RightVector() = glm::normalize(
			glm::cross(_cam.ForwardVector(), worldUpVector));
		_cam.UpVector() = glm::normalize(
			glm::cross(_cam.RightVector(), _cam.ForwardVector()));
	}

	void System_Camera::ForceUpdateModules(Modules& modules)
	{
		// I need to move this to the transform module
		if (modules.TypeModified<Module_Camera, Module_Transform>())
		{
			_modules = modules.
				GetModulesUnfiltered<Module_Camera, Module_Transform>();

			auto camMod = _modules[typeid(Module_Camera)].cbegin();
			auto tfMod = _modules[typeid(Module_Transform)].cbegin();

			auto camModEnd = _modules[typeid(Module_Camera)].cend();
			auto tfModEnd = _modules[typeid(Module_Transform)].cend();

			for (; camMod != camModEnd && tfMod != tfModEnd; ++camMod, ++tfMod)
			{
				auto cam = dynamic_cast<Module_Camera*>(*camMod);
				if (cam->currentCamera)
				{
					auto tf = dynamic_cast<Module_Transform*>(*tfMod);
					_cam = CameraStructure(cam, tf);
					return;
				}
			}
		}
	}

	void System_Camera::IncrementalUpdateModules(Modules& modules)
	{
		RemoveNullModules();

		// I need to move this to the transform module
		if (modules.TypeModified<Module_Camera, Module_Transform>())
		{
			_modules = modules.
				GetModulesFiltered<Module_Camera, Module_Transform>();
			for (auto mod : _modules[typeid(Module_Camera)])
			{
				auto cam = dynamic_cast<Module_Camera*>(mod);

				auto camMod = _modules[typeid(Module_Camera)].cbegin();
				auto tfMod = _modules[typeid(Module_Transform)].cbegin();

				auto camModEnd = _modules[typeid(Module_Camera)].cend();
				auto tfModEnd = _modules[typeid(Module_Transform)].cend();

				for (; camMod != camModEnd && tfMod != tfModEnd; 
					 ++camMod, ++tfMod)
				{
					auto cam = dynamic_cast<Module_Camera*>(*camMod);
					if (cam->currentCamera)
					{
						auto tf = dynamic_cast<Module_Transform*>(*tfMod);
						_cam = CameraStructure(cam, tf);
						return;
					}
				}
			}
		}
	}

	void System_Camera::DebugInfo()
	{
	}

}