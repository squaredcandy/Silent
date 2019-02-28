#pragma once

// Utility Stuff
#include "Silent/Utility/Random.h"

// For use by Silent applications
#include "Silent/Application.h"

// Platforms / Renderers
#include "Silent/Platform/LPlatformSDL.h"
#include "Silent/Renderer/LRendererGL3.h"

// Resources
#include "Silent/Resource/Resources.h"
#include "Silent/Resource/RShader.h"
#include "Silent/Resource/RMesh.h"
#include "Silent/Resource/RTexture.h"
#include "Silent/Resource/RMaterial.h"
#include "Silent/Resource/RBuffer.h"

/**
 * The Entity - Module - System Layering is as so (top to bottom):
 * Systems.h
 * System.h
 * LEntity.h
 * LModule.h
 * Module.h
 * Entity.h
 */
#include "Silent/Entity/LEntity.h"
#include "Silent/Module/LModule.h"
//#include "Silent/Module/LModule.cpp"
#include "Silent/Interface/Interface.h"
//#include "Silent/Interface/Interface.cpp"

//#include "Silent/System/Systems.h"
//#include "Silent/System/SSetting.h"
//#include "Silent/System/SInput.h"
//#include "Silent/System/SCamera.h"
//#include "Silent/System/SLight.h"
//#include "Silent/System/SDebug.h"
//#include "Silent/System/SRender.h"


//-----------Entry Point-----------
#include "Silent/EntryPoint.h"
//---------------------------------