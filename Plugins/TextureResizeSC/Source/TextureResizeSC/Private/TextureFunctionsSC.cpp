// Copyright 2022, Scionate Games - All Rights Reserved


#include "TextureFunctionsSC.h"

void UTextureFunctionsSC::PrintPixelFormat(UTexture2D* texture)
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, StaticEnum<ETextureSourceFormat>()->GetValueAsString(texture->Source.GetFormat()));
}
