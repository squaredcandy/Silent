#pragma once

#include "Silent/Utility/Singleton.h"

#include "Silent/Utility/Random.h"

// Include all the backends
#include "Silent/Platform/Platforms.h"

// For use by Silent applications
#include "Silent/Application.h"

#include "Silent/Resource/Resources.h"
#include "Silent/Resource/Resource_Shader.h"
#include "Silent/Resource/Resource_Mesh.h"
#include "Silent/Resource/Resource_Texture.h"
#include "Silent/Resource/Resource_Material.h"
#include "Silent/Resource/Resource_Buffer.h"

/**
 * The Entity - Module - System Layering is as so (top to bottom):
 * Systems.h
 * System.h
 * Entities.h
 * Modules.h
 * Module.h
 * Entity.h
 */
#include "Silent/Entity/Entities.h"
#include "Silent/Module/Modules.h"
//#include "Silent/Module/Modules.cpp"

#include "Silent/System/Systems.h"
#include "Silent/System/System_Camera.h"
#include "Silent/System/System_Render.h"


//-----------Entry Point-----------
#include "Silent/EntryPoint.h"
//---------------------------------