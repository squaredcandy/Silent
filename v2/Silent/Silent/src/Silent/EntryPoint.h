#pragma once
#include "Application.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
//#define DEBUG_NEW   new( _CLIENT_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
#endif // _DEBUG

#ifdef SL_PLATFORM_WINDOWS

extern Silent::Application * Silent::CreateApplication();

int main(int argc, char ** argv)
{
#ifdef _DEBUG
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	auto app = Silent::CreateApplication();
	app->Run();
	delete app;

#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return EXIT_SUCCESS;
}

#endif // SL_PLATFORM_WINDOWS
