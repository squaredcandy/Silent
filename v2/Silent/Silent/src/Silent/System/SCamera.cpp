#include "SCamera.h"
#include <SDL/SDL.h>

#include "../Interface/Interface.h"

namespace Silent
{
	void SCamera::Execute()
	{
		// Should have a better check for if we have a current camera or not
		if (!_cam._camera || !_cam._camtf) return;

		float dt = ImGui::GetIO().DeltaTime;

		CameraTranslation(dt);
		CameraRotation(dt);
	}

	void SCamera::Cleanup()
	{
		
	}

	void SCamera::CameraTranslation(float dt)
	{
		float moveSpeed = translationSpeed * dt;
		bool translated = false;
		auto newTranslation = _cam._camtf->translation;
		auto fd = _cam._camtf->forwardVector * moveSpeed;
		auto rt = _cam._camtf->rightVector * moveSpeed;
		auto up = _cam._camtf->upVector * moveSpeed;
		if (ImGui::IsKeyDown(SDL_SCANCODE_W))
		{
			newTranslation += fd;
			translated = true;
		}
		if (ImGui::IsKeyDown(SDL_SCANCODE_S))
		{
			newTranslation -= fd;
			translated = true;
		}
		if (ImGui::IsKeyDown(SDL_SCANCODE_D))
		{
			newTranslation += rt;
			translated = true;
		}
		if (ImGui::IsKeyDown(SDL_SCANCODE_A))
		{
			newTranslation -= rt;
			translated = true;
		}
		if (ImGui::IsKeyDown(SDL_SCANCODE_Q))
		{
			newTranslation += up;
			translated = true;
		}
		if (ImGui::IsKeyDown(SDL_SCANCODE_E))
		{
			newTranslation -= up;
			translated = true;
		}
		if (translated) ITransform::Translate(_cam._camtf, newTranslation);
	}

	void SCamera::CameraRotation(float dt)
	{
		if (!ImGui::IsMouseDown(1)) return;
		ImVec2 mouseDelta = ImGui::GetIO().MouseDelta;
		mouseDelta.x *= 0.01f;
		mouseDelta.y *= 0.01f;

		const float MaxLookHeight = 89.9f;
		auto rot = _cam._camtf->rotation;

		rot[0] += mouseDelta.x * rotationSpeed;
		rot[1] = glm::clamp(rot[1] - (mouseDelta.y * rotationSpeed),
							-MaxLookHeight, MaxLookHeight);
		ITransform::Rotate(_cam._camtf, rot);
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
				ITransform::UpdateRotationVectors(tf);
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