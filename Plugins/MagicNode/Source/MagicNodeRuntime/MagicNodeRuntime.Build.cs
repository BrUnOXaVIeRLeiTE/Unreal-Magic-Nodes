/// Copyright 2019 (C) Bruno Xavier B. Leite

using UnrealBuildTool;
using System.IO;

public class MagicNodeRuntime : ModuleRules {
	public MagicNodeRuntime(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Public/MagicNodeRuntime_Shared.h";
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