#include "System_Light.h"

namespace Silent
{

	std::vector<LightStruct> System_Light::GetLights(int num)
	{
		return std::vector<LightStruct>(_lights.begin(),
			(num < _lights.size()) ? 
				std::next(_lights.begin(), num) : 
				_lights.end()
		);
	}

	void System_Light::Execute()
	{
		if (_lights.size() == 1) return;
		if (cameraPos == _cameraSystem->GetCameras().Translation()) return;
		cameraPos = _cameraSystem->GetCameras().Translation();

		// Sort the lights by distance from the camera
		const auto& checkDistance = [=] (LightStruct& a, LightStruct& b)
		{
			auto aDist = glm::distance(cameraPos, a.Translation());
			auto bDist = glm::distance(cameraPos, b.Translation());
			return aDist < bDist;
		};

		std::sort(_lights.begin(), _lights.end(), checkDistance);
	}

	void System_Light::Cleanup()
	{

	}

	void System_Light::ForceUpdateModules(Modules& modules)
	{
		if (modules.TypeModified<Module_Light, Module_Transform>())
		{
			_modules = modules.
				GetModulesUnfiltered<Module_Light, Module_Transform>();

			auto size = _modules[typeid(Module_Transform)].size();

			auto tfBegin = _modules[typeid(Module_Transform)].cbegin();
			auto ltBegin = _modules[typeid(Module_Light)].cbegin();

			for (int i = 0; i < size; ++i)
			{
				auto tf = dynamic_cast<Module_Transform*>(*std::next(tfBegin, i));
				auto lt = dynamic_cast<Module_Light*>(*std::next(ltBegin, i));
				_lights.emplace_back(LightStruct{ tf, lt });
			}
		}
	}

	void System_Light::IncrementalUpdateModules(Modules& modules)
	{
		RemoveNullModules();

		if (modules.TypeModified<Module_Light, Module_Transform>())
		{
			_modules = modules.
				GetModulesFiltered<Module_Light, Module_Transform>();

			auto size = _modules[typeid(Module_Transform)].size();

			auto tfBegin = _modules[typeid(Module_Transform)].cbegin();
			auto ltBegin = _modules[typeid(Module_Light)].cbegin();

			for (int i = 0; i < size; ++i)
			{
				auto tf = dynamic_cast<Module_Transform*>(*std::next(tfBegin, i));
				auto lt = dynamic_cast<Module_Light*>(*std::next(ltBegin, i));
				_lights.emplace_back(LightStruct{ tf, lt });
			}
		}
	}

	template<typename T>
	void EmptySelectable(int& cSelected, std::string& cName, T *& cData)
	{
		if (ImGui::Selectable("", cSelected == -1))
		{
			cSelected = -1;
			cName = "";
			cData = nullptr;
		}
	}

	template<typename T>
	void DataSelectable(int& cSelected, std::string& cName, T *& cData, 
						int& nSelected, std::string nName, T * nData)
	{
		if (ImGui::Selectable(nName.c_str(), cSelected == nSelected))
		{
			cSelected = nSelected;
			cName = nName;
			cData = nData;
		}
	}

	void System_Light::DebugInfo()
	{
		static int cSelected = -1;
		static std::string cName;
		static LightStruct * cData = nullptr;
		if (ImGui::BeginCombo("Lights", cName.c_str()))
		{
			int i = 0;
			EmptySelectable<LightStruct>(cSelected, cName, cData);
			for (auto& light : _lights)
			{
				DataSelectable<LightStruct>(
					cSelected, cName, cData, i, light._tf->_name, &light);
			}

			ImGui::EndCombo();
		}

		if(cData != nullptr)
		{
			ImGui::DragFloat3("Translate", &cData->_tf->_translate[0], 0.1f);
			ImGui::DragFloat3("Rotate", &cData->_tf->_rotate[0], 0.1f);
			ImGui::DragFloat3("Scale", &cData->_tf->_scale[0], 0.1f);
		}

	}
}
