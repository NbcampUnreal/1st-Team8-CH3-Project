// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_GGF : ModuleRules
{
	public Project_GGF(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem", "AIModule", "GameplayTasks" });
	}
}
