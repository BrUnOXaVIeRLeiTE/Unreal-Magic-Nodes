//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IMagicNodeEditor.h"

#include "MGC_Toolkit.h"
#include "MGC_CodeEditorCore.h"

#include "MagicNodeEditor.h"
#include "MagicNodeEditorStyle.h"
#include "MagicNodeEditor_Shared.h"
#include "MagicNodeEditorCommands.h"

#include "Developer/AssetTools/Public/IAssetTools.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::StartupModule() {
	ToolBarExtensibilityManager = MakeShareable(new FExtensibilityManager);
	//
	IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	SY_AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Synaptech")),LOCTEXT("SynaptechCategory","Synaptech"));
	//
	{
		TSharedRef<IAssetTypeActions>ACT_MGC = MakeShareable(new FATA_MagicNode);
		AssetTools.RegisterAssetTypeActions(ACT_MGC);
	}
	//
	FMagicNodeEditorStyle::Initialize();
	FMagicNodeEditorCommands::Register();
	//
	MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("FileProject",EExtensionHook::After,TSharedPtr<FUICommandList>(),FMenuExtensionDelegate::CreateStatic(&FMagicNodeEditor::ExtendMenu));
	//
	FLevelEditorModule &LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	//
	//
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(TAB,FOnSpawnTab::CreateRaw(this,&FMagicNodeEditor::OnSpawnLightSourceViewerTAB))
	.SetDisplayName(LOCTEXT("MMC_SourceViewer","Source View"))
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
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TAB);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::ExtendMenu(FMenuBuilder &MenuBuilder) {
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

TSharedRef<SDockTab> FMagicNodeEditor::OnSpawnLightSourceViewerTAB(const FSpawnTabArgs &SpawnTabArgs) {
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

void FMagicNodeEditor::InvokeSourceViewerTAB() {
	TSharedRef<SDockTab>VIEW = FGlobalTabmanager::Get()->InvokeTab(TAB);
	//
	VIEW->SetTabIcon(FMagicNodeEditorStyle::Get()->GetBrush(TEXT("MagicNodeEditor.OpenSourceCodeViewer.Small")));
	VIEW->SetContent(
		SNew(SMGC_CodeEditorCore,nullptr)
		.ExternalSourcePath(ViewerSourcePath)
		.SourceToEdit(EMGC_CodeSource::Script)
	);//
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