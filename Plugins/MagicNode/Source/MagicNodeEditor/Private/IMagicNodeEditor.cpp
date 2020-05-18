//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IMagicNodeEditor.h"

#include "MGC_Toolkit.h"
#include "MGC_SearchFiles.h"
#include "MGC_EditorStyle.h"
#include "MGC_CodeEditorCore.h"
#include "MGC_EditorCommands.h"

#include "MagicNodeEditor.h"
#include "MagicNodeEditor_Shared.h"

#include "Developer/AssetTools/Public/IAssetTools.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::StartupModule() {
	FMagicNodeEditorStyle::Initialize();
	FMagicNodeEditorCommands::Register();
	//
	//
	IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	FLevelEditorModule &LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	SY_AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Synaptech")),LOCTEXT("SynaptechCategory","Synaptech"));
	//
	{
		TSharedRef<IAssetTypeActions>ACT_MGC = MakeShareable(new FATA_MagicNode);
		AssetTools.RegisterAssetTypeActions(ACT_MGC);
	}
	//
	//
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
	//
	MainMenuExtender = MakeShareable(new FExtender());
	MainMenuExtender->AddMenuExtension("FileProject",EExtensionHook::After,TSharedPtr<FUICommandList>(),FMenuExtensionDelegate::CreateStatic(&FMagicNodeEditor::ExtendMainMenu));
	//
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MainMenuExtender);
	//
	//
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TABSV,FOnSpawnTab::CreateRaw(this,&FMagicNodeEditor::OnSpawnSourceCodeViewerTAB))
	.SetDisplayName(LOCTEXT("MGC_SourceViewer","Source View"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
	//
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TABSS,FOnSpawnTab::CreateRaw(this,&FMagicNodeEditor::OnSpawnSourceCodeSearchTAB))
	.SetDisplayName(LOCTEXT("MGC_SourceSearch","Search in Files"))
	.SetMenuType(ETabSpawnerMenuType::Hidden);
	//
	//
	if (FMGC_Toolkit::SourceViewCount()==0) {
		FMGC_Toolkit::RefreshEngineSourceView();
		FMGC_Toolkit::RefreshPluginSourceView();
		FMGC_Toolkit::RefreshProjectSourceView();
	}///
}

void FMagicNodeEditor::ShutdownModule() {
	ToolBarExtensibilityManager.Reset();
	//
	FMagicNodeEditorStyle::Shutdown();
	FMagicNodeEditorCommands::Unregister();
	//
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TABSV);
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TABSS);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::ExtendMainMenu(FMenuBuilder &MenuBuilder) {
	MenuBuilder.AddMenuSeparator();
	//
	MenuBuilder.AddMenuEntry (
		LOCTEXT("MGC_NewScriptAsset","New Magic Node (C++)..." ),
		LOCTEXT("MGC_NewScriptAsset_Tooltip", "Create a new 'Magic Node' class.\nThis special class allows you to create custom functions,\ntyping code within user Blueprint Graphs executing the node.\nChanges to the node's source code will update all instances in all graphs."),
		FSlateIcon(FMagicNodeEditorStyle::Get().Get()->GetStyleSetName(),"ClassIcon.MagicNode"),
		FUIAction(FExecuteAction::CreateStatic(&FMagicNodeEditor::CreateNewScriptAsset))
	);///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<SDockTab> FMagicNodeEditor::OnSpawnSourceCodeViewerTAB(const FSpawnTabArgs &SpawnTabArgs) {
	TSharedRef<SDockTab>VIEW = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	//
	VIEW->SetTabIcon(FMagicNodeEditorStyle::Get()->GetBrush(TEXT("MagicNodeEditor.OpenSourceCodeViewer.Small")));
	VIEW->SetContent(
		SNew(SMGC_CodeEditorCore,nullptr)
		.ExternalSourcePath(ViewerSourcePath)
		.SourceToEdit(EMGC_CodeSource::Script)
	);//
	//
	return VIEW;
}

void FMagicNodeEditor::InvokeSourceCodeViewerTAB(const FTextLocation &Location) {
	TSharedRef<SDockTab>VIEW = FGlobalTabmanager::Get()->InvokeTab(TABSV);
	//
	TSharedRef<SMGC_CodeEditorCore>EDIT=SNew(SMGC_CodeEditorCore,nullptr)
	.SourceToEdit(EMGC_CodeSource::Script).ExternalSourcePath(ViewerSourcePath);
	//
	VIEW->SetTabIcon(FMagicNodeEditorStyle::Get()->GetBrush(TEXT("MagicNodeEditor.OpenSourceCodeViewer.Small")));
	VIEW->SetContent(EDIT); EDIT->GoToTextLocation(Location);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<SDockTab> FMagicNodeEditor::OnSpawnSourceCodeSearchTAB(const FSpawnTabArgs &SpawnTabArgs) {
	TSharedRef<SDockTab>SEARCH = SNew(SDockTab).TabRole(ETabRole::NomadTab);
	//
	SEARCH->SetTabIcon(FMagicNodeEditorStyle::Get()->GetBrush(TEXT("MagicNodeEditor.OpenSourceCodeSearch.Small")));
	SEARCH->SetContent(SNew(SMGC_SearchFiles));
	//
	return SEARCH;
}

void FMagicNodeEditor::InvokeSourceCodeSearchTAB() {
	TSharedRef<SDockTab>SEARCH = FGlobalTabmanager::Get()->InvokeTab(TABSS);
	SEARCH->SetContent(SNew(SMGC_SearchFiles));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::CreateNewScriptAsset() {
	IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	//
	FString PackageName = FString(TEXT("/Game/"));
	FString AssetName = FString(TEXT("NewMagicNode"));
	AssetTools.CreateUniqueAssetName(PackageName,AssetName,PackageName,AssetName);
	//
	UPackage* Package = CreatePackage(nullptr,*PackageName);
	UPackage* OuterPack = Package->GetOutermost();
	//
	auto ScriptFactory = NewObject<UMGC_ScriptFactory>();
	UObject* NewScript = ScriptFactory->FactoryCreateNew(UMagicNodeScript::StaticClass(),OuterPack,*AssetName,RF_Standalone|RF_Public,nullptr,GWarn);
	//
	FAssetRegistryModule::AssetCreated(NewScript);
	NewScript->MarkPackageDirty();
	NewScript->PostEditChange();
	NewScript->AddToRoot();
	//
	Package->SetDirtyFlag(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FMagicNodeEditor,MagicNodeEditor);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////