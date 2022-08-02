// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RetrowavePlatformer : ModuleRules
{
	public RetrowavePlatformer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
        { 
            "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore",
            "Niagara",
            "GameplayTasks",
            "NavigationSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[]
        {
            "RetrowavePlatformer/Public/Player",
            "RetrowavePlatformer/Public/Weapon",
            "RetrowavePlatformer/Public/Components",
            "RetrowavePlatformer/Public/AI",
            "RetrowavePlatformer/Public/Environment"
        });
        // Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
