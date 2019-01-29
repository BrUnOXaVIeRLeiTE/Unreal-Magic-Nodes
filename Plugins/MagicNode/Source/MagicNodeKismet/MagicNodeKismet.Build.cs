/// Copyright 2019 (C) Bruno Xavier B. Leite

using UnrealBuildTool;
using System.IO;

public class MagicNodeKismet : ModuleRules {
    public MagicNodeKismet(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Public/MagicNodeKismet_Shared.h";
		bEnforceIWYU = true;
		//
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "Engine",
                "CoreUObject",
                "MagicNodeRuntime"
            }///
        );//
        //
        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Slate",
                "Kismet",
                "UnrealEd",
                "Projects",
                "InputCore",
                "SlateCore",
				"GraphEditor",
                "LevelEditor",
                "EditorStyle",
                "PropertyEditor",
				"KismetCompiler",
				"BlueprintGraph"
            }///
        );//
        //
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory,"Public"));
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory,"Classes"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory,"Private"));
    }///
}