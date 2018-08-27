#include "Engine.h"
#include "TestSystem.h"

#include <iostream>

void LoadResources()
{
	using namespace Silent::Engine;
	Resources::LoadResourceFromFile<Mesh>("box", "box_export.FBX");
	Systems::AddSystem<TestSystem>();
	Entities::AddEntity("Test");

	auto m = Resources::GetResource<Mesh>("box");
	int i = 0;
}

int main(int argc, char ** argv)
{
	using namespace Silent;

	Engine::InitEngine();
	LoadResources();
	Engine::GameLoop();
	Engine::Cleanup();
	return 0;
}