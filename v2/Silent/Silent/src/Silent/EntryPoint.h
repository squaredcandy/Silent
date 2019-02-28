#pragma once
#include "Application.h"

#ifdef _DEBUG
//#define _CRTDBG_MAP_ALLOC
#include <vld.h>
#include <stdlib.h>
#include <crtdbg.h>
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DBG_NEW
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif // _DEBUG

#ifdef SL_PLATFORM_WINDOWS

extern Silent::Application * Silent::CreateApplication();

int main(int argc, char ** argv)
{
#ifdef _DEBUG
	// We do this flag to remove false positive from using _CrtDumpMemoryLeaks()
	_CrtSetDbgFlag (_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG

	auto app = Silent::CreateApplication();
	app->Run();
	delete app;

#ifdef _DEBUG
	// Not used because it false positives on static data
	//_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return EXIT_SUCCESS;
}

#endif // SL_PLATFORM_WINDOWS
