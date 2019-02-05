#pragma once
#include "../Core.h"
#include "System.h"
#include "SCamera.h"

namespace Silent
{
	struct LightStruct
	{
		std::shared_ptr<MTransform> _tf;
		std::shared_ptr<MLight> _light;

		LightStruct() :_tf(nullptr), _light(nullptr) {}
		//LightStruct(MTransform * tf, MLight * light)
		//	: _tf(tf), _light(light) {}
		LightStruct(std::shared_ptr<MTransform> tf, 
					std::shared_ptr<MLight> light)
			: _tf(tf), _light(light) {}
		~LightStruct() = default;
	};

	class SILENT_API SLight : public System
	{
	protected:
		std::vector<LightStruct> _lights;
		glm::vec3 cameraPos;

		void UpdateLights();

	public:
		SCamera * _cameraSystem;

		SLight(SystemPriority priority = 20)
			: System("Lights", priority) {}
		virtual ~SLight() = default;

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