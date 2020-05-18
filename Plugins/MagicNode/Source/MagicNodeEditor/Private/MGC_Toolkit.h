/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "KMGC_ScriptParser.h"

#include "MGC_SearchFiles.h"
#include "MGC_CodeEditorCore.h"
#include "MGC_EditorCommands.h"

#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Runtime/CoreUObject/Public/UObject/GCObject.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "Runtime/SlateCore/Public/Widgets/SBoxPanel.h"
#include "Runtime/SlateCore/Public/Widgets/Images/SImage.h"

#include "Runtime/Slate/Public/Widgets/Text/STextBlock.h"
#include "Runtime/Slate/Public/Widgets/Views/STreeView.h"
#include "Runtime/Slate/Public/Widgets/Input/SSearchBox.h"
#include "Runtime/Slate/Public/Widgets/Docking/SDockTab.h"

#include "Editor/UnrealEd/Public/Editor.h"
#include "Editor/UnrealEd/Public/EditorUndoClient.h"
#include "Editor/UnrealEd/Public/Toolkits/IToolkitHost.h"
#include "Editor/UnrealEd/Public/Toolkits/AssetEditorToolkit.h"

#include "Developer/DirectoryWatcher/Public/IDirectoryWatcher.h"
#include "Developer/DirectoryWatcher/Public/DirectoryWatcherModule.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMagicNodeScript;
class FMagicNodeEditor;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct FCodeEditorTAB {
	static const FName TAB_TreeView;
	static const FName TAB_Details;
	static const FName TAB_Script;
	static const FName TAB_Header;
	static const FName TAB_Types;
};

static FString ViewerSourcePath("C:/");

const FName FCodeEditorTAB::TAB_TreeView(TEXT("MGC_TreeView"));
const FName FCodeEditorTAB::TAB_Details(TEXT("MGC_Details"));
const FName FCodeEditorTAB::TAB_Script(TEXT("MGC_Script"));
const FName FCodeEditorTAB::TAB_Header(TEXT("MGC_Header"));
const FName FCodeEditorTAB::TAB_Types(TEXT("MGC_Types"));

const FName MGC_APP = FName(TEXT("MGC_CodeEditor"));

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Asset Editor Toolkit:

class FMGC_Toolkit : public IKMGC_ScriptParser, public FAssetEditorToolkit, public FEditorUndoClient, public FGCObject {
private:
	static FDelegateHandle WatcherHandle;
private:
	UMagicNodeScript* ScriptObject_Inline;
	TSharedPtr<SMGC_CodeEditorCore>MGC_CodeEditor;
	TArray<TSharedPtr<FSourceTreeNode>>SourceViewSearch;
	//
	TSharedPtr<FString>Search;
protected:
	TSharedRef<SDockTab>TABSpawn_TreeView(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Details(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Script(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Header(const FSpawnTabArgs &Args);
	TSharedRef<SDockTab>TABSpawn_Types(const FSpawnTabArgs &Args);
	//
	TSharedPtr<SSearchBox>SourceViewSearchBox;
	TSharedPtr<STreeView<TSharedPtr<FSourceTreeNode>>>SourceTreeWidget;
	TSharedPtr<STreeView<TSharedPtr<FSourceTreeNode>>>SourceSearchWidget;
	TSharedRef<ITableRow>OnGenerateSourceViewRow(TSharedPtr<FSourceTreeNode>InItem, const TSharedRef<STableViewBase>&OwnerTable);
	//
	EVisibility GetSourceTreeViewVisibility() const;
	EVisibility GetSourceTreeSearchVisibility() const;
	//
	void OnClickedSourceViewItem(TSharedPtr<FSourceTreeNode>TreeItem);
	void OnExpansionChanged(TSharedPtr<FSourceTreeNode>InItem, bool WasExpanded);
	void OnSelectedSourceViewItem(TSharedPtr<FSourceTreeNode>TreeItem, ESelectInfo::Type SelectInfo);
	void OnSourceViewCheckStatusChanged(ECheckBoxState NewCheckState, TSharedPtr<FSourceTreeNode>NodeChanged);
	void OnGetSourceViewChildren(TSharedPtr<FSourceTreeNode>InItem, TArray<TSharedPtr<FSourceTreeNode>>&OutChildren);
	//
	void ExtendMenu();
	void BindCommands();
	void ExtendToolbar();
protected:
	void CompileProject();
	void RebuildDatabases();
	void LaunchSourceCodeViewer();
	void LaunchSourceCodeSearch();
public:
	void CompileScript();
public:
	FString GetParentClass() const;
public:
	FMGC_Toolkit();
	//
	//
	UMagicNodeScript* GET() const;
	void SET(UMagicNodeScript* NewScriptObject);
	void INIT(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>&InitToolkitHost, UMagicNodeScript* ScriptObject);
	//
	//
	virtual void RegisterTabSpawners(const TSharedRef<FTabManager>&TBManager) override;
	virtual void UnregisterTabSpawners(const TSharedRef<FTabManager>&TBManager) override;
	//
	//
	virtual FText GetToolkitName() const override;
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetDocumentationLink() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	//
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>&Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>&Toolkit) override;
	//
	virtual void AddReferencedObjects(FReferenceCollector &Collector) override;
public:
	void OnSearchChanged(const FText &Filter);
	void OnProjectDirectoryChanged(const TArray<FFileChangeData> &Data);
	void OnSearchCommitted(const FText &NewText, ETextCommit::Type CommitInfo);
public:
	static void RefreshEngineSourceView();
	static void RefreshPluginSourceView();
	static void RefreshProjectSourceView();
	//
	void UpdateDatabaseReferences();
	//
	static bool IsSourceFile(const FString &Path);
	static int32 SourceViewCount() {return GlobalSourceTreeView.Num();}
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
