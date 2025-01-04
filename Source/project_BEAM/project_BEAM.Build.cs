// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class project_BEAM : ModuleRules
{
	public project_BEAM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "LocalMultiplayer", "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "AkAudio", "FCTween" });
	}
}
