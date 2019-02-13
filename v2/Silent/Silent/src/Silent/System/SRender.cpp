#include "SRender.h"

#include "../Resource/RMaterial.h"
#include "../Resource/RMesh.h"
#include "../Resource/RBuffer.h"
#include "../Interface/Interface.h"

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

	void SRender::UpdateModelMatrix()
	{
		for (auto& [key, model] : _models)
		{
			auto size = model.model.size();
			for (int i = 0; i < size; ++i)
			{
				if (ITransform::MatrixUpdated(model.tf[i]))
				{
					model.model[i] = model.tf[i]->modelMatrix;
				}
			}
		}
	}

	void SRender::SendModelMatrixData(Renderer * renderer,
									  std::shared_ptr<RMesh> mesh,
									  std::vector<glm::mat4>& model)
	{
		// We smaller than the batch size
		// We only do it once and dont have to copy vectors
		auto totalSize = (int) model.size();
		totalObjectsRendered += totalSize;
		if (totalSize < MaxBatchSize<glm::mat4>())
		{
			renderer->MapModelData(mesh->GetMeshID(), model);
			renderer->DrawModelInstanced(mesh->GetMeshID(), totalSize);
		}
		else
		{
			int splitIdx = 0;
			while (splitIdx * MaxBatchSize<glm::mat4>() < totalSize)
			{
				// Split the vector into smaller sizes
				const auto batchSize = MaxBatchSize<glm::mat4>();
				auto currentSplitSize = splitIdx * batchSize;
				auto nextSplitSize = (splitIdx + 1) * batchSize;
				std::vector<glm::mat4> sub (model.begin() + currentSplitSize, 
					(nextSplitSize < totalSize) ? 
					model.begin() + nextSplitSize : model.end());

				renderer->MapModelData(mesh->GetMeshID(), sub);
				renderer->DrawModelInstanced(mesh->GetMeshID(), totalSize);

				++splitIdx;
			}
		}
	}

	void SRender::Execute()
	{
		UpdateModelMatrix();
		bool a = true;
		ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		int totalObjectsRendered = 0;
		if (ImGui::Begin("Graphics Window", &a))
		{
			auto pos = ImGui::GetWindowPos();
			auto size = ImGui::GetWindowSize();

			auto & cam = _cameraSystem->GetCameras();
			auto projection = ICamera::ProjectionMatrix(cam._camera, size);
			auto view = ITransform::ViewMatrix(cam._camtf);

			BufferID currentBufferID = 0u;
			ShaderID currentShaderID = 0u;
			for (auto& [key, val] : _models)
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
				for (const auto& [name, var] : material->uniformParams)
				{
					shader->SetUniform(name, var);
				}

				shader->SetUniform("viewPos", cam._camtf->translation);
				// TODO: Slow - Redo this sometime
				std::vector<LightStruct> light = _lightSystem->GetLights(1);
				shader->SetUniform("lightPos", light[0]._tf->translation);

				// Assign all the textures
				for (const auto& [name, var] : material->_textures)
				{
					renderer->SetTexture(name, var->GetTextureID());
				}

				SendModelMatrixData(renderer, mesh, val.model);

				// Check if it is the last or 
				// the next iterator does not have the same buffer
				if (key == _models.rbegin()->first ||
					(*(_models.find(key)++)).first.buffer->GetBufferID() 
					!= currentBufferID)
				{
					buffer->End();
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();

		if(ImGui::Begin("Stats", &a))
		{
			static ImGuiIO& io = ImGui::GetIO();

			ImGui::Text("FrameTime %.3f (FPS %.3f)", 
						1000.f / io.Framerate, io.Framerate);
			ImGui::Text("No. Rendered Objects %d", totalObjectsRendered);

			ImGui::SliderFloat("Camera Speed", 
							   &_cameraSystem->translationSpeed, 0.1f, 10.f);

		}
		ImGui::End();
	}

	void SRender::Cleanup()
	{

	}

	void SRender::UpdateModels()
	{
		auto& renderModules = _modules[typeid(MRender)];
		auto size = renderModules.size();

		auto tfBegin = _modules[typeid(MTransform)].begin();
		auto mdlBegin = _modules[typeid(MModel)].begin();
		auto rdrBegin = _modules[typeid(MRender)].begin();

		for (auto i = 0; i < size; ++i)
		{
			auto rdr = std::dynamic_pointer_cast<MRender>(*std::next(rdrBegin, i));
			if (rdr->render)
			{
				auto tf = std::dynamic_pointer_cast<MTransform>(*std::next(tfBegin, i));
				auto mdl = std::dynamic_pointer_cast<MModel>(*std::next(mdlBegin, i));

				auto& matrixStruct = _models[{ rdr->buffer, mdl->mesh, mdl->material }];
				matrixStruct.tf.emplace_back(tf);
				matrixStruct.model.emplace_back(tf->modelMatrix);
			}
		}
	}

	void SRender::ForceUpdateModules(Modules& modules)
	{
		// Get the models
		if (modules.TypeModified<MTransform, MModel, MRender>())
		{
			_modules = modules.GetModules<MTransform, MModel, MRender>(false);
			UpdateModels();
		}
	}

	void SRender::IncrementalUpdateModules(Modules& modules)
	{
		//RemoveNullModules();

		// Get the models
		if (modules.TypeModified<MTransform, MModel, MRender>())
		{
			if (modules.WasAModuleRemovedThisFrame<MTransform, MModel, MRender>())
			{
				for (auto&[key, val] : _models)
				{
					
				}
			}
			_modules = modules.GetModules<MTransform, MModel, MRender>();
			UpdateModels();
		}
	}

	void SRender::DebugInfo()
	{
		if (ImGui::Button("Remove 0"))
		{
			auto& eee = Singleton<Entities>::Instance().GetEntities();
			Singleton<Modules>::Instance().RemoveModule<MModel>(eee[1]);
		}
	}
}