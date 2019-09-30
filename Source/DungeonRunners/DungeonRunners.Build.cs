// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DungeonRunners : ModuleRules
{
	public DungeonRunners(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayAbilities", "GameplayTags" });

  //      PrivateDependencyModuleNames.AddRange(
  //          new string[] {
  //              "GameplayAbilities"
  //              //"GameplayAbilitiesEditor",
  //              //"GameplayTags",
  //              //"GameplayTagsEditor",
  //              //"GameplayTasks"
  //          }
		//);
	}
}
