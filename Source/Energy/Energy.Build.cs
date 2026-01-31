// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Energy : ModuleRules
{
	public Energy(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
		PrivateDependencyModuleNames.AddRange(new string[] { "NavigationSystem", "Niagara", "AIModule"});
	}
}
