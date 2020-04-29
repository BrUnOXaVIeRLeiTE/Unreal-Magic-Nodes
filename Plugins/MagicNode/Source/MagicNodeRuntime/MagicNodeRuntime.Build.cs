/// Copyright 2020 (C) Bruno Xavier B. Leite

using UnrealBuildTool;
using System.IO;

public class MagicNodeRuntime : ModuleRules {
	public MagicNodeRuntime(ReadOnlyTargetRules Target) : base(Target) {
        PrivatePCHHeaderFile = "Public/MagicNodeRuntime_Shared.h";
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		//
		PublicDependencyModuleNames.AddRange(
			new string[] {
                "UMG",
				"Core",
                "Slate",
				"Engine",
                "InputCore",
                "SlateCore",
				"CoreUObject",
				"AssetRegistry"
			}///
		);//
		//
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory,"Public"));
		PrivateIncludePaths.Add(Path.Combine(ModuleDirectory,"Private"));
	}///
}