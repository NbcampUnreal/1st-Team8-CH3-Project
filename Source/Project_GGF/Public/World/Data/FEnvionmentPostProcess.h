// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FEnvionmentPostProcess.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FEnvironmentPostProcess
{
	GENERATED_BODY()

	// 오버라이드 플래그들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_WhiteTemp:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_ColorSaturation:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_ColorContrast:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_ColorGamma:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_ColorGain:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_ColorOffset:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_AutoExposureMinBrightness:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overrides, meta=(PinHiddenByDefault, InlineEditConditionToggle))
	uint8 bOverride_AutoExposureMaxBrightness:1;

	
	// 포스트 프로세스
	UPROPERTY(interp, BlueprintReadWrite, Category="Color Grading|Temperature", meta=(UIMin = "1500.0", UIMax = "15000.0", editcondition = "bOverride_WhiteTemp", DisplayName = "Temp"))
	float WhiteTemp;
	
	// 컬러 그레이딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color Grading|Global", meta = (UIMin = "0.0", UIMax = "2.0", Delta = "0.01", ColorGradingMode = "saturation", SupportDynamicSliderMaxValue = "true", editcondition = "bOverride_ColorSaturation", DisplayName = "Saturation"))
	FVector4 ColorSaturation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Color Grading|Global", meta = (UIMin = "0.0", UIMax = "2.0", Delta = "0.01", ColorGradingMode = "contrast", SupportDynamicSliderMaxValue = "true", editcondition = "bOverride_ColorContrast", DisplayName = "Contrast"))
	FVector4 ColorContrast;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Color Grading|Global", meta = (UIMin = "0.0", UIMax = "2.0", Delta = "0.01", ColorGradingMode = "gamma", SupportDynamicSliderMaxValue = "true", editcondition = "bOverride_ColorGamma", DisplayName = "Gamma"))
	FVector4 ColorGamma;

	UPROPERTY(interp, BlueprintReadWrite, Category = "Color Grading|Global", meta = (UIMin = "0.0", UIMax = "2.0", Delta = "0.01", ColorGradingMode = "gain", SupportDynamicSliderMaxValue = "true", editcondition = "bOverride_ColorGain", DisplayName = "Gain"))
	FVector4 ColorGain;
	
	UPROPERTY(interp, BlueprintReadWrite, Category = "Color Grading|Global", meta = (UIMin = "-1.0", UIMax = "1.0", Delta = "0.001", ColorGradingMode = "offset", SupportDynamicSliderMaxValue = "true", SupportDynamicSliderMinValue = "true", editcondition = "bOverride_ColorOffset", DisplayName = "Offset"))
	FVector4 ColorOffset;

	// 노출
	UPROPERTY(interp, BlueprintReadWrite, Category="Lens|Exposure", meta=(ClampMin = "-10.0", UIMax = "20.0", editcondition = "bOverride_AutoExposureMinBrightness", DisplayName = "Min Brightness"))
	float AutoExposureMinBrightness = -10.0f;

	UPROPERTY(interp, BlueprintReadWrite, Category="Lens|Exposure", meta=(ClampMin = "-10.0", UIMax = "20.0", editcondition = "bOverride_AutoExposureMaxBrightness", DisplayName = "Max Brightness"))
	float AutoExposureMaxBrightness = 20.0f;

	// 함수
	// 생성자 - 기본값 초기화
	FEnvironmentPostProcess()
	{
		// 오버라이드 플래그 초기화
		bOverride_WhiteTemp = false;
		bOverride_ColorSaturation = false;
		bOverride_ColorContrast = false;
		bOverride_ColorGamma = false;
		bOverride_ColorGain = false;
		bOverride_ColorOffset = false;
		bOverride_AutoExposureMinBrightness = false;
		bOverride_AutoExposureMaxBrightness = false;

		// 기본값 설정
		WhiteTemp = 6500.0f;
		ColorSaturation = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		ColorContrast = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		ColorGamma = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		ColorGain = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
		ColorOffset = FVector4(0.0f, 0.0f, 0.0f, 0.0f);
		AutoExposureMinBrightness = 0.03f;
		AutoExposureMaxBrightness = 2.0f;
	}
	
	// 연산자
	void operator = (const FEnvironmentPostProcess& Other)
	{
		// 오버라이드 플래그 복사
        bOverride_WhiteTemp = Other.bOverride_WhiteTemp;
        bOverride_ColorSaturation = Other.bOverride_ColorSaturation;
        bOverride_ColorContrast = Other.bOverride_ColorContrast;
        bOverride_ColorGamma = Other.bOverride_ColorGamma;
        bOverride_ColorGain = Other.bOverride_ColorGain;
        bOverride_ColorOffset = Other.bOverride_ColorOffset;
        bOverride_AutoExposureMinBrightness = Other.bOverride_AutoExposureMinBrightness;
        bOverride_AutoExposureMaxBrightness = Other.bOverride_AutoExposureMaxBrightness;

        // 값 복사
        WhiteTemp = Other.WhiteTemp;
        ColorSaturation = Other.ColorSaturation;
        ColorContrast = Other.ColorContrast;
        ColorGamma = Other.ColorGamma;
        ColorGain = Other.ColorGain;
        ColorOffset = Other.ColorOffset;
        AutoExposureMinBrightness = Other.AutoExposureMinBrightness;
        AutoExposureMaxBrightness = Other.AutoExposureMaxBrightness;
    }

	/*
	 * @brief 포스트 프로세스 세팅 구조체로 변환하는 함수
	 */
	FPostProcessSettings ToPostProcessSettings() const
	{
		FPostProcessSettings Result;
        
		// 오버라이드 플래그와 값 설정
		if (bOverride_WhiteTemp)
		{
			Result.bOverride_WhiteTemp = true;
			Result.WhiteTemp = WhiteTemp;
		}
        
		if (bOverride_ColorSaturation)
		{
			Result.bOverride_ColorSaturation = true;
			Result.ColorSaturation = ColorSaturation;
		}
        
		if (bOverride_ColorContrast)
		{
			Result.bOverride_ColorContrast = true;
			Result.ColorContrast = ColorContrast;
		}
        
		if (bOverride_ColorGamma)
		{
			Result.bOverride_ColorGamma = true;
			Result.ColorGamma = ColorGamma;
		}
        
		if (bOverride_ColorGain)
		{
			Result.bOverride_ColorGain = true;
			Result.ColorGain = ColorGain;
		}
        
		if (bOverride_ColorOffset)
		{
			Result.bOverride_ColorOffset = true;
			Result.ColorOffset = ColorOffset;
		}
        
		if (bOverride_AutoExposureMinBrightness)
		{
			Result.bOverride_AutoExposureMinBrightness = true;
			Result.AutoExposureMinBrightness = AutoExposureMinBrightness;
		}
        
		if (bOverride_AutoExposureMaxBrightness)
		{
			Result.bOverride_AutoExposureMaxBrightness = true;
			Result.AutoExposureMaxBrightness = AutoExposureMaxBrightness;
		}
        
		return Result;
	}

	/*
	 * @brief 포스트 프로세스 값을 다른 구조체와 알파값으로 보간하는 함수
	 */
	FEnvironmentPostProcess LerpWith(const FEnvironmentPostProcess& Other, float Alpha) const
	{
	    FEnvironmentPostProcess Result;
	    
	    // 포스트 프로세스 값 보간
	    Result.WhiteTemp = FMath::Lerp(WhiteTemp, Other.WhiteTemp, Alpha);
	    
	    // 컬러 그레이딩 값 보간
	    Result.ColorSaturation = FMath::Lerp(ColorSaturation, Other.ColorSaturation, Alpha);
	    Result.ColorContrast = FMath::Lerp(ColorContrast, Other.ColorContrast, Alpha);
	    Result.ColorGamma = FMath::Lerp(ColorGamma, Other.ColorGamma, Alpha);
	    Result.ColorGain = FMath::Lerp(ColorGain, Other.ColorGain, Alpha);
	    Result.ColorOffset = FMath::Lerp(ColorOffset, Other.ColorOffset, Alpha);
	    
	    // 노출 값 보간
	    Result.AutoExposureMinBrightness = FMath::Lerp(AutoExposureMinBrightness, Other.AutoExposureMinBrightness, Alpha);
	    Result.AutoExposureMaxBrightness = FMath::Lerp(AutoExposureMaxBrightness, Other.AutoExposureMaxBrightness, Alpha);
	    
	    // 오버라이드 플래그 설정 - 두 구조체 중 하나라도 오버라이드되어 있으면 결과도 오버라이드
	    Result.bOverride_WhiteTemp = bOverride_WhiteTemp || Other.bOverride_WhiteTemp;
	    Result.bOverride_ColorSaturation = bOverride_ColorSaturation || Other.bOverride_ColorSaturation;
	    Result.bOverride_ColorContrast = bOverride_ColorContrast || Other.bOverride_ColorContrast;
	    Result.bOverride_ColorGamma = bOverride_ColorGamma || Other.bOverride_ColorGamma;
	    Result.bOverride_ColorGain = bOverride_ColorGain || Other.bOverride_ColorGain;
	    Result.bOverride_ColorOffset = bOverride_ColorOffset || Other.bOverride_ColorOffset;
	    Result.bOverride_AutoExposureMinBrightness = bOverride_AutoExposureMinBrightness || Other.bOverride_AutoExposureMinBrightness;
	    Result.bOverride_AutoExposureMaxBrightness = bOverride_AutoExposureMaxBrightness || Other.bOverride_AutoExposureMaxBrightness;
	    
	    return Result;
	}

	/*
	 * @brief FPostProcessSettings와 알파값으로 보간하는 함수
	 */
	FPostProcessSettings LerpWithPostProcessSettings(const FPostProcessSettings& Other, float Alpha) const
	{
	    FPostProcessSettings Result;
	    
	    // 포스트 프로세스 값 보간 (오버라이드된 값만 보간)
	    if (bOverride_WhiteTemp || Other.bOverride_WhiteTemp)
	    {
	        Result.bOverride_WhiteTemp = true;
	        Result.WhiteTemp = FMath::Lerp(WhiteTemp, Other.WhiteTemp, Alpha);
	    }
	    
	    if (bOverride_ColorSaturation || Other.bOverride_ColorSaturation)
	    {
	        Result.bOverride_ColorSaturation = true;
	        Result.ColorSaturation = FMath::Lerp(ColorSaturation, Other.ColorSaturation, Alpha);
	    }
	    
	    if (bOverride_ColorContrast || Other.bOverride_ColorContrast)
	    {
	        Result.bOverride_ColorContrast = true;
	        Result.ColorContrast = FMath::Lerp(ColorContrast, Other.ColorContrast, Alpha);
	    }
	    
	    if (bOverride_ColorGamma || Other.bOverride_ColorGamma)
	    {
	        Result.bOverride_ColorGamma = true;
	        Result.ColorGamma = FMath::Lerp(ColorGamma, Other.ColorGamma, Alpha);
	    }
	    
	    if (bOverride_ColorGain || Other.bOverride_ColorGain)
	    {
	        Result.bOverride_ColorGain = true;
	        Result.ColorGain = FMath::Lerp(ColorGain, Other.ColorGain, Alpha);
	    }
	    
	    if (bOverride_ColorOffset || Other.bOverride_ColorOffset)
	    {
	        Result.bOverride_ColorOffset = true;
	        Result.ColorOffset = FMath::Lerp(ColorOffset, Other.ColorOffset, Alpha);
	    }
	    
	    if (bOverride_AutoExposureMinBrightness || Other.bOverride_AutoExposureMinBrightness)
	    {
	        Result.bOverride_AutoExposureMinBrightness = true;
	        Result.AutoExposureMinBrightness = FMath::Lerp(AutoExposureMinBrightness, Other.AutoExposureMinBrightness, Alpha);
	    }
	    
	    if (bOverride_AutoExposureMaxBrightness || Other.bOverride_AutoExposureMaxBrightness)
	    {
	        Result.bOverride_AutoExposureMaxBrightness = true;
	        Result.AutoExposureMaxBrightness = FMath::Lerp(AutoExposureMaxBrightness, Other.AutoExposureMaxBrightness, Alpha);
	    }
	    
	    return Result;
	}
	
};
