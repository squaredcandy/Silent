#pragma once
#include "../Core.h"
#include "System.h"
#include "System_Camera.h"

namespace Silent
{
	struct LightStruct
	{
		Module_Transform * _tf;
		Module_Light * _light;

		LightStruct() :_tf(nullptr), _light(nullptr) {}
		LightStruct(Module_Transform * tf, Module_Light * light)
			: _tf(tf), _light(light) {}
		~LightStruct() = default;

		glm::vec3& Translation() { return _tf->_translate; }
	};

	class SILENT_API System_Light : public System
	{
	protected:
		std::vector<LightStruct> _lights;
		glm::vec3 cameraPos;
	public:
		System_Camera * _cameraSystem;

		System_Light(SystemPriority priority = 20)
			: System("Lights", priority) {}
		virtual ~System_Light() = default;

		std::vector<LightStruct>& GetLights() { return _lights; }
		// This is to return a number of lights
		std::vector<LightStruct> GetLights(int num);

		virtual void Execute() override;
		virtual void Cleanup() override;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;

		virtual void DebugInfo() override;
	};
}