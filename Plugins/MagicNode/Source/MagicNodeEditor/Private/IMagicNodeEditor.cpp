//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IMagicNodeEditor.h"

#include "MagicNodeEditor_Shared.h"
#include "MagicNodeEditorStyle.h"
#include "MGC_CodeEditorStyle.h"
#include "MagicNodeEditor.h"

#include "Developer/AssetTools/Public/IAssetTools.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::StartupModule() {
	FMagicNodeEditorStyle::Initialize();
	FMGC_CodeEditorStyle::Initialize();
	//
	//
	IAssetTools &AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
	SY_AssetCategory = AssetTools.RegisterAdvancedAssetCategory(FName(TEXT("Synaptech")),LOCTEXT("SynaptechCategory","Synaptech"));
	//
	{
		TSharedRef<IAssetTypeActions>ACT_MGCS = MakeShareable(new FATA_MGCScript);
		AssetTools.RegisterAssetTypeActions(ACT_MGCS);
	}
	//
	//
	MenuExtender = MakeShareable(new FExtender());
	MenuExtender->AddMenuExtension("FileProject",EExtensionHook::First,TSharedPtr<FUICommandList>(),FMenuExtensionDelegate::CreateStatic(&FMagicNodeEditor::ExtendMenu));
	//
	FLevelEditorModule &LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FMagicNodeEditor::ShutdownModule() {
	FMGC_CodeEditorStyle::Shutdown();
	FMagicNodeEditorStyle::Shutdown();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMagicNodeEditor::ExtendMenu(FMenuBuilder &MenuBuilder) {
	MenuBuilder.AddMenuEntry (
		LOCTEXT("MGC_NewScriptAsset","New Magic Node Class..." ),
		LOCTEXT("MGC_NewScriptAsset_Tooltip", "Create a new 'Magic Node' class.\nThis special class allows you to create custom functions,\ntyping code within user Blueprint Graphs executing the node.\nChanges to the node's source code will update all instances in all graphs."),
		FSlateIcon(FMagicNodeEditorStyle::Get().Get()->GetStyleSetName(),"ClassIcon.MagicNodeScript"),
		FUIAction(FExecuteAction::CreateStatic(&FMagicNodeEditor::CreateNewScriptAsset))
	);///
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