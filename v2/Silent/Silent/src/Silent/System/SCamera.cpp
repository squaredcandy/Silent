#include "SCamera.h"
#include <SDL/SDL.h>

namespace Silent
{
	void SCamera::Execute()
	{
		// Should have a better check for if we have a current camera or not
		if (!_cam._camera || !_cam._camtf) return;

		float dt = ImGui::GetIO().DeltaTime;

		CameraTranslation(dt);
		CameraRotation(dt);
		UpdateCameraVectors();
	}

	void SCamera::Cleanup()
	{
		
	}

	glm::vec3 SCamera::GetFaceCameraVector()
	{
		return glm::vec3(0, _cam.Rotation().x + 90.f, 0);
	}

	glm::mat4 SCamera::GetProjectionMatrix(ImVec2& size)
	{
		return glm::perspective(_cam._camera->fov, size.x / size.y,
							_cam._camera->nearPlane, _cam._camera->farPlane);
	}

	glm::mat4 SCamera::GetViewMatrix()
	{
		return glm::lookAt(_cam.Translation(),
						   _cam.Translation() + _cam.ForwardVector(),
						   _cam.UpVector());
	}

	void SCamera::CameraTranslation(float dt)
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

	void SCamera::CameraRotation(float dt)
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

	void SCamera::UpdateCameraVectors()
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

	void SCamera::UpdateCamera()
	{
		auto camMod = _modules[typeid(MCamera)].cbegin();
		auto tfMod = _modules[typeid(MTransform)].cbegin();

		auto camModEnd = _modules[typeid(MCamera)].cend();
		auto tfModEnd = _modules[typeid(MTransform)].cend();

		for (; camMod != camModEnd && tfMod != tfModEnd; ++camMod, ++tfMod)
		{
			auto cam = std::dynamic_pointer_cast<MCamera>(*camMod);
			if (cam->currentCamera)
			{
				auto tf = std::dynamic_pointer_cast<MTransform>(*tfMod);
				_cam = CameraStructure(cam, tf);
				return;
			}
		}
	}
	

	void SCamera::ForceUpdateModules(Modules& modules)
	{
		// I need to move this to the transform module
		if (modules.TypeModified<MCamera, MTransform>())
		{
			_modules = modules.
				GetModules<MCamera, MTransform>();
			_modules = modules.GetModules<MCamera>();
			UpdateCamera();
		}
	}

	void SCamera::IncrementalUpdateModules(Modules& modules)
	{
		RemoveNullModules();

		// I need to move this to the transform module
		if (modules.TypeModified<MCamera, MTransform>())
		{
			_modules = modules.
				GetModules<MCamera, MTransform>();
			UpdateCamera();
		}
	}

	void SCamera::DebugInfo()
	{
	}

}