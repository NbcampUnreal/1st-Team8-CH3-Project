// Copyright 2022, Scionate Games - All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Engine/Classes/Engine/TextureRenderTarget2D.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "TextureFunctionsSC.generated.h"


/**
 * 
 */
UCLASS()
class TEXTURERESIZESC_API UTextureFunctionsSC : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:

		UFUNCTION(BlueprintCallable, Category = "Rendering")
		static void PrintPixelFormat(UTexture2D* texture);
};
