//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "IMagicNodeRuntime.h"
#include "MagicNodeRuntime.h"

#if WITH_EDITOR
  #include "ISettingsModule.h"
  #include "ISettingsSection.h"
  #include "ISettingsContainer.h"
#endif

#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FMagicNodeRuntime : public IMagicNodeRuntime {
private:
	bool HandleSettingsSaved() {
	  #if WITH_EDITOR
		const auto &Settings = GetMutableDefault<UMGC_Settings>();
		Settings->SaveConfig(); return true;
	  #endif
	return false;}
	//
	void RegisterSettings() {
	  #if WITH_EDITOR
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
			ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");
			SettingsContainer->DescribeCategory("Synaptech",LOCTEXT("SynaptechCategoryName","Synaptech"),
			LOCTEXT("SynaptechCategoryDescription","Configuration of Synaptech Systems."));
			//
			ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings("Project","Synaptech","MGC_Settings",
				LOCTEXT("MGC_SettingsName","Magic Node Settings"),
				LOCTEXT("MGC_SettingsDescription","General Settings for the Magic Node Plugin"),
			GetMutableDefault<UMGC_Settings>());
			//
			if (SettingsSection.IsValid()) {SettingsSection->OnModified().BindRaw(this,&FMagicNodeRuntime::HandleSettingsSaved);}
		}///
	  #endif
	}///
	//
	void UnregisterSettings() {
	  #if WITH_EDITOR
		if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings")) {
			SettingsModule->UnregisterSettings("Project","Synaptech","MGC_Settings");
		}///
	  #endif
	}///
	//
public:
	virtual void StartupModule() override {
		UE_LOG(MagicNodes,Log,TEXT("Initializing Magic Nodes..."));
		RegisterSettings();
		//
		FAssetRegistryModule &AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
		TArray<FAssetData>NodeData; AssetRegistry.Get().GetAssetsByClass(UMagicNode::StaticClass()->GetFName(),NodeData);
		for (FAssetData Data : NodeData) {Data.GetAsset();}
		//
		#if WITH_EDITOR
		TArray<FAssetData>ScriptData; AssetRegistry.Get().GetAssetsByClass(UMagicNodeScript::StaticClass()->GetFName(),ScriptData);
		for (FAssetData Data : ScriptData) {Data.GetAsset();}
		#endif
	}///
	//
	virtual void ShutdownModule() override {if (UObjectInitialized()) {UnregisterSettings();}}
	virtual bool SupportsDynamicReloading() override {return false;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_GAME_MODULE(FMagicNodeRuntime,MagicNodeRuntime);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////