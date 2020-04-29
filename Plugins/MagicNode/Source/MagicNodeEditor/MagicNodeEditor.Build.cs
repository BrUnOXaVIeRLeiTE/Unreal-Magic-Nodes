/// Copyright 2020 (C) Bruno Xavier B. Leite

using UnrealBuildTool;
using System.IO;

public class MagicNodeEditor : ModuleRules {
    public MagicNodeEditor(ReadOnlyTargetRules Target) : base(Target) {
        PrivatePCHHeaderFile = "Public/MagicNodeEditor_Shared.h";
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		bEnforceIWYU = true;
		//
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "Slate",
                "Engine",
                "SlateCore",
                "InputCore",
                "CoreUObject",
                "MagicNodeKismet",
                "MagicNodeRuntime",
                "DirectoryWatcher"
            }///
        );//
        //
        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Kismet",
                "UnrealEd",
                "Projects",
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