// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameJamCPP : ModuleRules
{
	public GameJamCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GameJamCPP",
			"GameJamCPP/Variant_Platforming",
			"GameJamCPP/Variant_Platforming/Animation",
			"GameJamCPP/Variant_Combat",
			"GameJamCPP/Variant_Combat/AI",
			"GameJamCPP/Variant_Combat/Animation",
			"GameJamCPP/Variant_Combat/Gameplay",
			"GameJamCPP/Variant_Combat/Interfaces",
			"GameJamCPP/Variant_Combat/UI",
			"GameJamCPP/Variant_SideScrolling",
			"GameJamCPP/Variant_SideScrolling/AI",
			"GameJamCPP/Variant_SideScrolling/Gameplay",
			"GameJamCPP/Variant_SideScrolling/Interfaces",
			"GameJamCPP/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
