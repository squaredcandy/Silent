#include "Engine.h"

#include <iostream>

int main(int argc, char ** argv)
{
	Silent::Engine::InitEngine(BACKEND_GL);
	Silent::Engine::GameLoop();
	Silent::Engine::Cleanup();
	return 0;
}