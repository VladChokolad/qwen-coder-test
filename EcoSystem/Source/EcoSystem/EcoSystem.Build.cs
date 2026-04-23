// Copyright Epic Games, Inc. All Rights Reserved.

#include "EcoSystem.Build.cs"

public class EcoSystem : ModuleRules
{
	public EcoSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.PCHOrSharedThroughHeader;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"NavigationSystem",
			"AIModule"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		DynamicallyLoadedModuleNames.AddRange(new string[] { });
	}
}
