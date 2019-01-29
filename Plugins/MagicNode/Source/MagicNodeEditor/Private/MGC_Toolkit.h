/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MGC_CodeEditorCore.h"

#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Public/EditorUndoClient.h"
#include "Runtime/CoreUObject/Public/UObject/GCObject.h"
#include "Editor/UnrealEd/Public/Toolkits/IToolkitHost.h"
#include "Runtime/Slate/Public/Widgets/Docking/SDockTab.h"
#include "Editor/UnrealEd/Public/Toolkits/AssetEditorToolkit.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMagicNodeScript;

struct FCodeEditorTAB {
	static const FName TAB_Details;
	static const FName TAB_Script;
	static const FName TAB_Header;
	static const FName TAB_Types;
};

const FName FCodeEditorTAB::TAB_Details(TEXT("MGC_Details"));
const FName FCodeEditorTAB::TAB_Script(TEXT("MGC_Script"));
const FName FCodeEditorTAB::TAB_Header(TEXT("MGC_Header"));
const FName FCodeEditorTAB::TAB_Types(TEXT("MGC_Types"));

const FName MGC_APP = FName(TEXT("MGC_CodeEditor"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC State-Machine Asset Editor Toolkit:

class FMGC_Toolkit : public FAssetEditorToolkit, public FEditorUndoClient, public FGCObject {
private:
	/** MGC Script Object being edited. */
	UMagicNodeScript* ScriptObject_Inline;
	//
	/** MGC Code Editor Widget. */
	TSharedPtr<SMGC_CodeEditorCore>MGC_CodeEditor;
protected:
	TSharedRef<SDockTab>TABSpawn_Details(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Script(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Header(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Types(const FSpawnTabArgs &Args);
	//
	//void ExtendMenu();
	//void BindCommands();
	//void ExtendToolbar();
public:
	FMGC_Toolkit();
	//
	//
	void INIT_CodeEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>&InitToolkitHost, UMagicNodeScript* ScriptObject);
	//
	/** Sets the State-Machine being edited. */
	void SET_MGC_Inline(UMagicNodeScript* NewScriptObject);
	//
	/** Gets the State-Machine being edited. */
	UMagicNodeScript* GET_MGC_Inline() const;
	//
	//
	// IToolkit interface
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>&TBManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>&TBManager) override;
	// End of IToolkit interface
	//
	// FAssetEditorToolkit
	virtual FText GetToolkitName() const override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>&Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>&Toolkit) override;
	// End of FAssetEditorToolkit
	//
	// FSerializableObject interface
	virtual void AddReferencedObjects(FReferenceCollector &Collector) override;
	// End of FSerializableObject interface
};

/* /// Graph Notification System::
static void MGC_HandleHyperlinkNavigate() {
	FGlobalTabmanager::Get()->InvokeTab(FName("OutputLog"));
}/////////
	//
	FNotificationInfo NInfo = FNotificationInfo(FText::FromString(TEXT("Derp!  DEERRRRPPPP !!!!!!!!!!!!!!!")));
	NInfo.ExpireDuration = 6.5f;
	NInfo.bUseThrobber = true;
	NInfo.bFireAndForget = true;
	NInfo.bUseSuccessFailIcons = true;
	NInfo.Image = MagicNodeStyle::GetBrush("ClassThumbnail.MGC_Event");
	NInfo.Hyperlink = FSimpleDelegate::CreateStatic(&MGC_HandleHyperlinkNavigate);
	NInfo.HyperlinkText = NSLOCTEXT("MGC_ScriptPanel", "DEERRRRPPPP", "Open DEERRRRPPPP Folder...");
	MGC_CodeEditor->AddNotification(NInfo,true);
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
