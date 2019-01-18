#include <Silent.h>

#include <glm/matrix.hpp>

using namespace Silent;
class Sandbox : public Silent::Application
{
protected:
	glm::vec3 pos{ 0, 0, 0 };

	FastNoise fn1 = FastNoise(Random::RandomInt(1000));
	FastNoise fn2 = FastNoise(Random::RandomInt(1000));
	FastNoise fn3 = FastNoise(Random::RandomInt(1000));

	std::shared_ptr<Resource_Mesh> mesh;
	std::shared_ptr<Resource_Texture> tex;
	std::shared_ptr<Resource_Material> mat;
	std::shared_ptr<Resource_Buffer> buffer;

	void SandboxInitFunction()
	{
 		syss->AddSystem<System_Camera>();
 		syss->AddSystem<System_Render>();

		buffer = ress->LoadResource<Resource_Buffer>("DrawBuffer", platform->renderer);
		auto shader = ress->LoadResource<Resource_Shader>("sprite", platform->renderer);
		mesh = ress->LoadResource<Resource_Mesh>("box.obj", platform->renderer);
		tex = ress->LoadResource<Resource_Texture>("m.png", platform->renderer);

		mat = ress->LoadResource<Resource_Material>("Material");
		mat->SetShader(shader);
		mat->AddTexture(0, tex);
		mat->AddParameter("sprite", 0);
 
 		auto cam = ents->AddEntity("Camera");
 
 		auto modCam = mods->AddModule<Module_Camera>(cam);
 		modCam->currentCamera = true;
 
 		auto box = ents->AddEntity("Box");
 		auto model = mods->AddModule<Module_Model>(box);
 		model->mesh = mesh;
 		model->material = mat;
 
 		auto modRender = mods->AddModule<Module_Render>(box);
 		modRender->buffer = buffer;
 		fn1.SetFrequency(0.2f);
 		fn2.SetFrequency(0.2f);
 		fn3.SetFrequency(0.2f);

// 		for (int i = 0; i < 4096; ++i)
// 		{
// 			auto box2 = ents->AddEntity("Box2" + std::to_string(i));
// 			auto tf2 = mods->GetModule<Module_Transform>(box2);
// 			const float offset = 0.1f;
// 			const float cutoff = 0.01f;
// 			float x = 2 * fn1.GetPerlin(pos.x, pos.y, pos.z);
// 			float y = 2 * fn2.GetPerlin(pos.x, pos.y, pos.z);
// 			float z = 2 * fn3.GetPerlin(pos.x, pos.y, pos.z);
// 
// 			if (x < cutoff && x > -cutoff) x = 0;
// 			if (y < cutoff && y > -cutoff) y = 0;
// 			if (z < cutoff && z > -cutoff) z = 0;
// 
// 			if (x == 0 && y == 0 && z == 0)
// 			{
// 				int i = 0;
// 			}
// 
// 			tf2->_translate.x = x;
// 			tf2->_translate.y = y;
// 			tf2->_translate.z = z;
// 
// 			const float scaleSize = 0.001f;
// 			tf2->_scale = { scaleSize, scaleSize, scaleSize };
// 			pos += offset;
// 
// 			auto model2 = mods->AddModule<Module_Model>(box2);
// 			model2->mesh = mesh;
// 			model2->material = mat;
// 
// 			auto modRender2 = mods->AddModule<Module_Render>(box2);
// 			modRender2->buffer = buffer;
// 		}
 	}
 
 	void SandboxLoopFunction()
 	{
		int i = 0;
 		syss->RunSystems();
		
		static uint32_t timer = 0;
		timer += SDL_GetTicks();
		if (timer > 2000)
		{
			auto box2 = ents->AddEntity("Box2" + std::to_string(i));
			auto tf2 = mods->GetModule<Module_Transform>(box2);
			const float offset = 0.1f;
			const float cutoff = 0.01f;
			float x = 2 * fn1.GetPerlin(pos.x, pos.y, pos.z);
			float y = 2 * fn2.GetPerlin(pos.x, pos.y, pos.z);
			float z = 2 * fn3.GetPerlin(pos.x, pos.y, pos.z);

			if (x < cutoff && x > -cutoff) x = 0;
			if (y < cutoff && y > -cutoff) y = 0;
			if (z < cutoff && z > -cutoff) z = 0;

			if (x == 0 && y == 0 && z == 0)
			{
				int i = 0;
			}

			tf2->_translate.x = x;
			tf2->_translate.y = y;
			tf2->_translate.z = z;

			const float scaleSize = 0.001f;
			tf2->_scale = { scaleSize, scaleSize, scaleSize };
			pos += offset;

			auto model2 = mods->AddModule<Module_Model>(box2);
			model2->mesh = mesh;
			model2->material = mat;

			auto modRender2 = mods->AddModule<Module_Render>(box2);
			modRender2->buffer = buffer;
		}
	}

public:
	Sandbox() : Silent::Application()
	{
		platformType = SL_PLATFORM_SDL;
		rendererType = SL_RENDERER_GL3;
		initFunction = [this] { SandboxInitFunction(); };
		loopFunction = [this] { SandboxLoopFunction(); };
	}

	virtual ~Sandbox()
	{
	}
};

Silent::Application * Silent::CreateApplication()
{
	return new Sandbox();
}