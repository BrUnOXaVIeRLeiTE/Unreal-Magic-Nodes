//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_EditorStyle.h"
#include "MagicNodeEditor_Shared.h"

#include "Styling/SlateStyle.h"
#include "Interfaces/IPluginManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define PLUGIN_BRUSH(RelativePath,...) FSlateImageBrush(FMagicNodeEditorStyle::InContent(RelativePath,TEXT(".png")),__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedPtr<FSlateStyleSet>FMagicNodeEditorStyle::StyleSet = nullptr;
TSharedPtr<ISlateStyle>FMagicNodeEditorStyle::Get() {return StyleSet;}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FMagicNodeEditorStyle::InContent(const FString &RelativePath, const TCHAR* Extension) {
	static FString Content = IPluginManager::Get().FindPlugin(MGC_PLUGIN_NAME)->GetContentDir();
	return (Content/RelativePath)+Extension;
}

FName FMagicNodeEditorStyle::GetStyleSetName() {
	static FName StyleName(TEXT("MagicNodeEditorStyle"));
	return StyleName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditorStyle::Initialize() {
	if (StyleSet.IsValid()) {return;}
	//
	const FVector2D Icon16x16(16.f,16.f);
	const FVector2D Icon20x20(16.f,16.f);
	const FVector2D Icon40x40(40.f,40.f);
	const FVector2D Icon128x128(128.f,128.f);
	//
	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(MGC_PLUGIN_NAME)->GetContentDir());
	//
	//
	StyleSet->Set("ClassIcon.MagicNodeScript", new PLUGIN_BRUSH(TEXT("Icons/MagicNode_16x"),Icon16x16));
	StyleSet->Set("ClassThumbnail.MagicNodeScript", new PLUGIN_BRUSH(TEXT("Icons/MagicNode_128x"),Icon128x128));
	//
	StyleSet->Set("ClassIcon.MagicNode", new PLUGIN_BRUSH(TEXT("Icons/MagicNode_16x"),Icon16x16));
	StyleSet->Set("ClassThumbnail.MagicNode", new PLUGIN_BRUSH(TEXT("Icons/MagicNode_128x"),Icon128x128));
	//
	StyleSet->Set("Toolbar.MagicNodeScript", new PLUGIN_BRUSH(TEXT("Icons/MagicNode_128x"),Icon40x40));
	//
	StyleSet->Set("SourceView.FolderClosed", new PLUGIN_BRUSH(TEXT("Icons/FolderClosed_16x"),Icon16x16));
	StyleSet->Set("SourceView.FolderOpened", new PLUGIN_BRUSH(TEXT("Icons/FolderOpened_16x"),Icon16x16));
	StyleSet->Set("SourceView.HeaderFile", new PLUGIN_BRUSH(TEXT("Icons/HeaderFile_16x"),Icon16x16));
	StyleSet->Set("SourceView.ConfigFile", new PLUGIN_BRUSH(TEXT("Icons/ConfigFile_16x"),Icon16x16));
	StyleSet->Set("SourceView.BinaryFile", new PLUGIN_BRUSH(TEXT("Icons/BinaryFile_16x"),Icon16x16));
	StyleSet->Set("SourceView.Library", new PLUGIN_BRUSH(TEXT("Icons/Library_16x"),Icon16x16));
	StyleSet->Set("SourceView.CppFile", new PLUGIN_BRUSH(TEXT("Icons/CppFile_16x"),Icon16x16));
	StyleSet->Set("SourceView.Plugins", new PLUGIN_BRUSH(TEXT("Icons/Plugins_16x"),Icon16x16));
	StyleSet->Set("SourceView.Project", new PLUGIN_BRUSH(TEXT("Icons/Project_16x"),Icon16x16));
	StyleSet->Set("SourceView.CsFile", new PLUGIN_BRUSH(TEXT("Icons/CsFile_16x"),Icon16x16));
	StyleSet->Set("SourceView.Engine", new PLUGIN_BRUSH(TEXT("Icons/Engine_16x"),Icon16x16));
	StyleSet->Set("SourceView.Object", new PLUGIN_BRUSH(TEXT("Icons/Object_16x"),Icon16x16));
	StyleSet->Set("SourceView.Text", new PLUGIN_BRUSH(TEXT("Icons/Text_16x"),Icon16x16));
	//
	StyleSet->Set("MagicNodeEditor.Compile", new PLUGIN_BRUSH(TEXT("UI/KMGC_Compile_40x"),Icon40x40));
	StyleSet->Set("MagicNodeEditor.Compile.Small", new PLUGIN_BRUSH(TEXT("UI/KMGC_Compile_40x"),Icon20x20));
	//
	StyleSet->Set("MagicNodeEditor.BuildDatabase", new PLUGIN_BRUSH(TEXT("UI/KMGC_Database_40x"),Icon40x40));
	StyleSet->Set("MagicNodeEditor.BuildDatabase.Small", new PLUGIN_BRUSH(TEXT("UI/KMGC_Database_40x"),Icon20x20));
	//
	StyleSet->Set("MagicNodeEditor.OpenSourceCodeViewer", new PLUGIN_BRUSH(TEXT("UI/KMGC_SourceCode_40x"),Icon40x40));
	StyleSet->Set("MagicNodeEditor.OpenSourceCodeViewer.Small", new PLUGIN_BRUSH(TEXT("UI/KMGC_SourceCode_40x"),Icon20x20));
	//
	StyleSet->Set("MagicNodeEditor.OpenSourceCodeSearch", new PLUGIN_BRUSH(TEXT("UI/KMGC_FindFiles_40x"),Icon40x40));
	StyleSet->Set("MagicNodeEditor.OpenSourceCodeSearch.Small", new PLUGIN_BRUSH(TEXT("UI/KMGC_FindFiles_40x"),Icon20x20));
	//
	//
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

void FMagicNodeEditorStyle::Shutdown() {
	if (StyleSet.IsValid()) {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique()); StyleSet.Reset();
	}///
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef PLUGIN_BRUSH

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////