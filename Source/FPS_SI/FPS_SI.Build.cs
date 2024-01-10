// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPS_SI : ModuleRules
{
	public FPS_SI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine" });

        PrivateDependencyModuleNames.AddRange(new string[] { "AIModule", "EnhancedInput", "GameplayAbilities", "GameplayTags", "GameplayTasks", "InputCore" });
    }
}
