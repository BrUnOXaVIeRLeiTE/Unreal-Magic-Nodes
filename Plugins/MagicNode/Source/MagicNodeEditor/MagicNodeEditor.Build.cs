/// Copyright 2019 (C) Bruno Xavier B. Leite

using UnrealBuildTool;
using System.IO;

public class MagicNodeEditor : ModuleRules {
    public MagicNodeEditor(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "Public/MagicNodeEditor_Shared.h";
		bEnforceIWYU = true;
		//
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "Engine",
                "CoreUObject",
                "MagicNodeKismet",
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
                "AssetTools",
				"GraphEditor",
                "LevelEditor",
                "EditorStyle",
				"KismetWidgets",
                "PropertyEditor",
				"KismetCompiler",
				"BlueprintGraph"
            }///
        );//
        //
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory,"Public"));
        PrivateIncludePaths.Add(Path.Combine(ModuleDirectory,"Private"));
    }///
}