#include "System_Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Resource/Resource_Material.h"
#include "../Resource/Resource_Mesh.h"
#include "../Resource/Resource_Buffer.h"

namespace Silent
{
	bool ModelStructure::operator==(const ModelStructure& other) const
	{
		return buffer->GetBufferID() == other.buffer->GetBufferID() &&
			mesh->GetMeshID() == other.mesh->GetMeshID() &&
			material->GetMaterialID() == other.material->GetMaterialID();
	}

	bool ModelStructure::operator<(const ModelStructure& other) const
	{
		return buffer->GetBufferID() < other.buffer->GetBufferID() &&
			mesh->GetMeshID() < other.mesh->GetMeshID() &&
			material->GetMaterialID() < other.material->GetMaterialID();
	}

	// This is slow, as this remove all the entites and adds them all back in 
	// again adding alot of redundancy when a relevant module every frame
	// This should be changed to force update entities as a last resort so it 
	// flushes all the previous modules and starts again
	void System_Render::UpdateEntities(Modules& modules)
	{
		// Get the camera we are working with
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
					break;
				}
			}
		}

		// Get the models
		if (modules.TypeModified<Module_Transform, Module_Model, Module_Render>())
		{
			auto newMods = modules.GetModulesAddedThisFrame
				<Module_Transform, Module_Model, Module_Render>(true);
			_modules = modules.GetModulesFiltered
				<Module_Transform, Module_Model, Module_Render>(newMods);
			auto& renderModules = _modules[typeid(Module_Render)];
			auto size = renderModules.size();

			auto tfBegin = _modules[typeid(Module_Transform)].begin();
			auto mdlBegin = _modules[typeid(Module_Model)].begin();
			auto rdrBegin = _modules[typeid(Module_Render)].begin();

			for (auto i = 0; i < size; ++i)
			{
				auto rdr = dynamic_cast<Module_Render*>(*std::next(rdrBegin, i));
				if (rdr->render)
				{
					auto tf = dynamic_cast<Module_Transform*>(*std::next(tfBegin, i));
					auto mdl = dynamic_cast<Module_Model*>(*std::next(mdlBegin, i));

					_models[{ rdr->buffer, mdl->mesh, mdl->material }].
						emplace_back(GetModelMatrix(tf));
				}
			}
		}
	}

	glm::vec3 System_Render::GetFaceCameraVector()
	{
		return glm::vec3(0, _camera->rotation.x + 90.f, 0);
	}

	glm::mat4 System_Render::GetProjectionMatrix(ImVec2& size)
	{
		return glm::perspective(_camera->fov, size.x / size.y, 
								_camera->nearPlane, _camera->farPlane);
	}

	glm::mat4 System_Render::GetViewMatrix()
	{
		return glm::lookAt(_camera->translation, 
						   _camera->translation + _camera->forwardVector, 
						   _camera->upVector);
	}

	glm::mat4 System_Render::GetModelMatrix(const Module_Transform * tf)
	{
		const glm::vec3 xRot{ 1, 0, 0 };
		const glm::vec3 yRot{ 0, 1, 0 };
		const glm::vec3 zRot{ 0, 0, 1 };

		// Translate -> Scale -> Rotate
		auto model = glm::translate(glm::mat4(), tf->_translate);
		model = glm::scale(model, tf->_scale);
		model = glm::rotate(model, glm::radians(tf->_rotate.x), xRot);
		model = glm::rotate(model, glm::radians(tf->_rotate.y), yRot);
		model = glm::rotate(model, glm::radians(tf->_rotate.z), zRot);
		return model;
	}

	void System_Render::Execute()
	{
		bool a = true;
		ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Graphics Window", &a);

		auto pos = ImGui::GetWindowPos();
		auto size = ImGui::GetWindowSize();

		auto projection = GetProjectionMatrix(size);
		auto view = GetViewMatrix();

		BufferID currentBufferID = 0u;
		ShaderID currentShaderID = 0u;
		for (const auto& [key, val] : _models)
		{
			const auto& buffer = key.buffer;
			// if we arent switching buffers we use the last buffer
			if (currentBufferID != buffer->GetBufferID())
			{
				buffer->Begin();
				currentBufferID = buffer->GetBufferID();
			}

			const auto& material = key.material;
			const auto& shader = material->_shader;
			// If we arent switching shaders we use the last shader
			if (currentShaderID != shader->GetShaderID())
			{
				shader->SetActive();
				currentShaderID = shader->GetShaderID();

				shader->SetUniform("Projection", projection);
				shader->SetUniform("View", view);
			}

			const auto& mesh = key.mesh;
			const auto& renderer = shader->GetRenderer();

			// Set each shaders uniform
			// possibly a bug here were we dont set all the default params
			for (const auto& [key, val] : material->uniformParams)
			{
				shader->SetUniform(key, val);
			}

			// Assign all the textures
			for (const auto& [key, val] : material->_textures)
			{
				renderer->SetTexture(key, val->GetTextureID());
			}

			// We smaller than the batch size
			// We only do it once and dont have to copy vectors
			int totalSize = (int)val.size();
			if (totalSize < MaxBatchSize)
			{
				renderer->MapModelData(mesh->GetMeshID(), val);
				renderer->DrawModelInstanced(mesh->GetMeshID(), totalSize);
			}
			else
			{
				int splitIdx = 0;
				while (splitIdx * MaxBatchSize < totalSize)
				{
					// Split the vector into smaller sizes
					std::vector<glm::mat4> sub
					(
						val.begin() + (splitIdx * MaxBatchSize),
						((splitIdx + 1) * MaxBatchSize < totalSize) ?
						val.begin() + (splitIdx + 1) * MaxBatchSize : val.end()
					);

					renderer->MapModelData(mesh->GetMeshID(), sub);
					renderer->DrawModelInstanced(mesh->GetMeshID(), totalSize);

					++splitIdx;
				}
			}

			// Check if it is the last or the next iterator does not have the same buffer
			if (key == _models.rbegin()->first || 
				(*(_models.find(key)++)).first.buffer->GetBufferID() != currentBufferID)
			{
				buffer->End();
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::ShowDemoWindow(&a);
	}

	void System_Render::Cleanup()
	{

	}

	void System_Render::ForceUpdateModules(Modules& modules)
	{
		UpdateEntities(modules);
	}

	void System_Render::IncrementalUpdateModules(Modules& modules)
	{
		// Remove all the nullptrs incase we removed any
// 		for (auto& [key, val] : _modules)
// 		{
// 			val.erase(remove_if(val.begin(), val.end(), [] (const AModule& x) { return x == nullptr; }), val.end());
// 		}

		for (auto& [key, val] : _modules)
		{
			for (auto cont = val.begin(); cont != val.end();)
			{
				if ((*cont) == nullptr) cont = val.erase(cont);
				else ++cont;
			}
		}

		// Add modules added last frame
		// So the idea is that since we emplace back the new modules
		// we know all the new modules are at the end
		// So if we know the size of the vector last frame 
		// (should the current size of our corresponding vector after nullptr deletions)
		// we just have to get the subset that is not in our current vector

		if (modules.TypeModified<Module_Camera>())
		{
		}

		if (modules.TypeModified<Module_Transform, Module_Model, Module_Render>())
		{

		}
	}

}