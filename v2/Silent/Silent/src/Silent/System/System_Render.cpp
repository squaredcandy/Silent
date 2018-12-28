#include "System_Render.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Resource/Resource_Material.h"
#include "../Resource/Resource_Mesh.h"
#include "../Resource/Resource_Buffer.h"

namespace Silent
{
	void System_Render::UpdateEntities(Modules& modules)
	{
		// Get the camera we are working with
		if (modules.TypeModified<Module_Camera>())
		{
			_modules = modules.GetModules<Module_Camera>(true);
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
			_models.clear();
			_modules = modules.GetModules<Module_Transform, 
				Module_Model, Module_Render>(true);
			auto& renderModules = _modules[typeid(Module_Render)];
			auto size = renderModules.size();
			_models.reserve(size);

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
					_models.emplace_back(ModelStructure{ tf, mdl, rdr });
				}
			}

			// We need to sort it by bufferID and then shaderID
			std::sort(_models.begin(), _models.end(), 
					[] (const ModelStructure& lhs, const ModelStructure& rhs) {
				
				// We order by the bufferID first
				// This ensures that same buffer objects are being rendered
				// together and not overwritten
				auto eqlBuf = lhs.render->buffer->GetBufferID() <
					rhs.render->buffer->GetBufferID();
				if (eqlBuf) return eqlBuf;
				
				// We order by shaderID second
				// This minimises shader swapping which is expensive apparently
				auto eqlShd = lhs.model->material->_shader->GetShaderID() <
					lhs.model->material->_shader->GetShaderID();
				if (eqlShd) return eqlShd;

				// We order by textureID third
				// Texture swapping is also expensive
				return false;
			});
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

		auto model = glm::translate(glm::mat4(), tf->_translate);
		model = glm::rotate(model, glm::radians(tf->_rotate.x), xRot);
		model = glm::rotate(model, glm::radians(tf->_rotate.y), yRot);
		model = glm::rotate(model, glm::radians(tf->_rotate.z), zRot);
		model = glm::scale(model, tf->_scale);
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
		for (const auto& _model : _models)
		{
			const auto& buffer = _model.render->buffer;
			// if we arent switching buffers we use the last buffer
			if (currentBufferID != buffer->GetBufferID())
			{
				buffer->Begin();
				currentBufferID = buffer->GetBufferID();
			}

			const auto& material = _model.model->material;
			const auto& shader = material->_shader;
			// If we arent switching shaders we use the last shader
			if (currentShaderID != shader->GetShaderID())
			{
				shader->SetActive();
				currentShaderID = shader->GetShaderID();

				shader->SetUniform("Projection", projection);
				shader->SetUniform("View", view);
			}

			const auto& tf = _model.transform;
			const auto& mesh = _model.model->mesh;
			const auto& renderer = shader->GetRenderer();

			// Set each shaders uniform
			// possibly a bug here were we dont set all the default params
			for (const auto& [key, val] : material->uniformParams)
			{
				shader->SetUniform(key, val);
			}

			// Set the model matrix
			auto model = GetModelMatrix(tf);
			shader->SetUniform("Model", model);

			// Assign all the textures
			for (const auto& [key, val] : material->_textures)
			{
				renderer->SetTexture(key, val->GetTextureID());
			}
			// Draw the model
			// This is making things super expensive
			// Need to do some form of batch rendering here
			renderer->DrawModel(mesh->GetMeshID());
			
			// If it is the last model or if the next one is not the same we 
			// end buffer
			if ((&_model == &_models.back()) || 
				(*(&_model)).render->buffer->GetBufferID() != currentBufferID)
			{
				buffer->End();
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::ShowDemoWindow(&a);
	}
}