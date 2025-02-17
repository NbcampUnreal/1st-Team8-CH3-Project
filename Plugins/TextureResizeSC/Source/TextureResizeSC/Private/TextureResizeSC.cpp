// Copyright 2022, Scionate Games - All Rights Reserved


#include "TextureResizeSC.h"

#define LOCTEXT_NAMESPACE "FTextureTResizeSCModule"

void FTextureResizeSCModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FTextureResizeSCModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FTextureResizeSCModule, TextureResizeSC)