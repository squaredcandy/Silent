#include "System_Camera.h"

#include <ImGui/imgui.h>
#include <SDL/SDL.h>

namespace Silent
{
	void System_Camera::Execute()
	{
		// Should have a better check for if we have a current camera or not
		if (!_camera) return;

		float dt = ImGui::GetIO().DeltaTime;

		CameraTranslation(dt);
		CameraRotation(dt);
		UpdateCameraVectors();
	}

	void System_Camera::Cleanup()
	{
		
	}

	void System_Camera::CameraTranslation(float dt)
	{
		float moveSpeed = _camera->translateSpeed * dt;
		if (ImGui::IsKeyDown(SDL_SCANCODE_W))
			_camera->translation += _camera->forwardVector * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_S))
			_camera->translation -= _camera->forwardVector * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_D))
			_camera->translation += _camera->rightVector * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_A))
			_camera->translation -= _camera->rightVector * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_Q))
			_camera->translation += _camera->upVector * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_E))
			_camera->translation -= _camera->upVector * moveSpeed;
	}

	void System_Camera::CameraRotation(float dt)
	{
		if (!ImGui::IsMouseDown(1)) return;
		ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
		mouseDelta.x *= 0.01f;
		mouseDelta.y *= 0.01f;

		const float MaxLookHeight = 89.9f;
		
		float rotSpeed = _camera->rotateSpeed;
		_camera->rotation[0] += mouseDelta.x * rotSpeed;
		_camera->rotation[1] = glm::clamp(_camera->rotation[1] - 
			(mouseDelta.y * rotSpeed), -MaxLookHeight, MaxLookHeight);
	}

	void System_Camera::UpdateCameraVectors()
	{
		auto yawRad = glm::radians(_camera->rotation[0]);
		auto pitchRad = glm::radians(_camera->rotation[1]);
		auto cYawRad = cosf(yawRad);
		auto cPitchRad = cosf(pitchRad);
		auto sYawRad = sinf(yawRad);
		auto sPitchRad = sinf(pitchRad);

		const glm::vec3 worldUpVector{ 0.f, 1.f, 0.f };

		auto forward = glm::vec3(cYawRad * cPitchRad, sPitchRad, sYawRad * cPitchRad);
		_camera->forwardVector = glm::normalize(forward);
		_camera->rightVector = glm::normalize(
			glm::cross(_camera->forwardVector, worldUpVector));
		_camera->upVector = glm::normalize(
			glm::cross(_camera->rightVector, _camera->forwardVector));
	}

	void System_Camera::ForceUpdateModules(Modules& modules)
	{
		// I need to move this to the transform module
		if (modules.TypeModified<Module_Camera>())
		{
			_modules = modules.GetModulesUnfiltered<Module_Camera>(true);
			for (auto mod : _modules[typeid(Module_Camera)])
			{
				auto cam = dynamic_cast<Module_Camera*>(mod);
				if (cam->currentCamera)
				{
					// we use the first current camera we can find
					_camera = cam;
					return;
				}
			}
		}
	}

	void System_Camera::IncrementalUpdateModules(Modules& modules)
	{
		for (auto&[key, val] : _modules)
		{
			for (auto cont = val.begin(); cont != val.end();)
			{
				if ((*cont) == nullptr) cont = val.erase(cont);
				else ++cont;
			}
		}

		// I need to move this to the transform module
		if (modules.TypeModified<Module_Camera>())
		{
			auto newMods = modules.GetModulesAddedThisFrame<Module_Camera>(true);
			_modules = modules.GetModulesFiltered<Module_Camera>(newMods);
			for (auto mod : _modules[typeid(Module_Camera)])
			{
				auto cam = dynamic_cast<Module_Camera*>(mod);
				if (cam->currentCamera)
				{
					// we use the first current camera we can find
					_camera = cam;
					return;
				}
			}
		}
	}

}