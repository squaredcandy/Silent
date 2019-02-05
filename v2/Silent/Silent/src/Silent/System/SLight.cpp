#include "SLight.h"

namespace Silent
{

	std::vector<LightStruct> SLight::GetLights(int num)
	{
		return std::vector<LightStruct>(_lights.begin(),
			(num < _lights.size()) ? 
				std::next(_lights.begin(), num) : 
				_lights.end()
		);
	}

	void SLight::Execute()
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

	void SLight::Cleanup()
	{

	}

	void SLight::UpdateLights()
	{
		auto size = _modules[typeid(MTransform)].size();

		auto tfBegin = _modules[typeid(MTransform)].cbegin();
		auto ltBegin = _modules[typeid(MLight)].cbegin();

		for (int i = 0; i < size; ++i)
		{
			auto tf = std::dynamic_pointer_cast<MTransform>(*std::next(tfBegin, i));
			auto lt = std::dynamic_pointer_cast<MLight>(*std::next(ltBegin, i));
			_lights.emplace_back(LightStruct{ tf, lt });
		}
	}

	void SLight::ForceUpdateModules(Modules& modules)
	{
		if (modules.TypeModified<MLight, MTransform>())
		{
			_modules = modules.
				GetModules<MLight, MTransform>();
			UpdateLights();
		}
	}

	void SLight::IncrementalUpdateModules(Modules& modules)
	{
		RemoveNullModules();

		if (modules.TypeModified<MLight, MTransform>())
		{
			_modules = modules.
				GetModules<MLight, MTransform>();
			UpdateLights();
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

	void SLight::DebugInfo()
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
			if (ImGui::DragFloat3("Translate", &cData->_tf->_translate[0], 0.1f))
			{
				cData->_tf->updateMatrix = true;
			}
			ImGui::DragFloat3("Rotate", &cData->_tf->_rotate[0], 0.1f);
			ImGui::DragFloat3("Scale", &cData->_tf->_scale[0], 0.1f);
		}

	}
}
