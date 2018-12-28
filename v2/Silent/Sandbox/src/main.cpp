#include <Silent.h>

#include <glm/matrix.hpp>

using namespace Silent;
class Sandbox : public Silent::Application
{
protected:
	Resources * ress;
	Entities * ents;
	Modules * mods;
	Systems * syss;

	void SandboxInitFunction()
	{
		syss->AddSystem<System_Camera>();
		syss->AddSystem<System_Render>();

		auto buffer = ress->LoadResource<Resource_Buffer>("DrawBuffer", platform->renderer);
		auto shader = ress->LoadResource<Resource_Shader>("sprite", platform->renderer);
		auto mesh = ress->LoadResource<Resource_Mesh>("box.obj", platform->renderer);
		auto tex = ress->LoadResource<Resource_Texture>("m.png", platform->renderer);

		auto mat = ress->LoadResource<Resource_Material>("Material");
		mat->SetShader(shader);
		mat->AddTexture(0, tex);
		mat->AddParameter("sprite", 0);

		auto box = ents->AddEntity("Box");
		auto cam = ents->AddEntity("Camera", box);

		auto modCam = mods->AddModule<Module_Camera>(cam);
		modCam->currentCamera = true;

		auto model = mods->AddModule<Module_Model>(box);
		model->mesh = mesh;
		model->material = mat;

		auto modRender = mods->AddModule<Module_Render>(box);
		modRender->buffer = buffer;

		for (int i = 0; i < 10; ++i)
		{
			auto box2 = ents->AddEntity("Box2" + std::to_string(i));
			auto tf2 = mods->GetModule<Module_Transform>(box2);
			tf2->_translate = { i, i, i };

			auto model2 = mods->AddModule<Module_Model>(box2);
			model2->mesh = mesh;
			model2->material = mat;

			auto modRender2 = mods->AddModule<Module_Render>(box2);
			modRender2->buffer = buffer;
		}
	}

	void SandboxLoopFunction()
	{
		int i = 0;
		syss->RunSystems();
	}

public:
	Sandbox() : Silent::Application()
	{
		platformType = SL_PLATFORM_SDL;
		rendererType = SL_RENDERER_GL3;
		initFunction = [this] { SandboxInitFunction(); };
		loopFunction = [this] { SandboxLoopFunction(); };
		
		ress = &Singleton<Resources>::Instance();
		ents = &Singleton<Entities>::Instance();
		mods = &Singleton<Modules>::Instance();
		syss = &Singleton<Systems>::Instance();
	}

	~Sandbox()
	{
		ress->Cleanup();
	}
};

Silent::Application * Silent::CreateApplication()
{
	return new Sandbox();
}