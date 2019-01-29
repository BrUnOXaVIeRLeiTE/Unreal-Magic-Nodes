//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_Toolkit.h"
#include "MagicNodeEditor_Shared.h"

#include "Editor/KismetWidgets/Public/SSingleObjectDetailsPanel.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC State-Machine Toolkit Constructor:

FMGC_Toolkit::FMGC_Toolkit(){}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC State-Machine Details Native API:

class SMGC_PropertyTab : public SSingleObjectDetailsPanel {
private:
	TWeakPtr<FMGC_Toolkit>CodeEditor;
public:
	SLATE_BEGIN_ARGS(SMGC_PropertyTab){}
	SLATE_END_ARGS()
	//
	//
	void Construct(const FArguments &InArgs,TSharedPtr<FMGC_Toolkit>MGC_Toolkit) {
		CodeEditor = MGC_Toolkit;
		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(MGC_Toolkit->GetToolkitCommands()).HostTabManager(MGC_Toolkit->GetTabManager()),true,true);
	}///
	//
	virtual UObject* GetObjectToObserve() const override {
		return CodeEditor.Pin()->GET_MGC_Inline();
	}///
	//
	virtual TSharedRef<SWidget>PopulateSlot(TSharedRef<SWidget>PropertyEditorWidget) override {
		return SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.FillHeight(1)
		[
			PropertyEditorWidget
		];
	}///
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC State-Machine Toolkit Native API:

void FMGC_Toolkit::INIT_CodeEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>&InitToolkitHost, UMagicNodeScript* ScriptObject) {
	FAssetEditorManager::Get().CloseOtherEditors(ScriptObject,this);
	//
	ScriptObject_Inline = ScriptObject;
	check(ScriptObject_Inline);
	//
	ScriptObject_Inline->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);
	//
	//
	/*FTileMapEditorCommands::Register();
	BindCommands();*/
	//
	/*ToolBoxWidget = SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("ToolPanel.GroupBorder"))
		.Padding(0.f);*/
	//
	//
	// Default layout
	const TSharedRef<FTabManager::FLayout>FCodeEditorLayout = FTabManager::NewLayout("MGC_CodeEditorLayout_V002")
	->AddArea
	(
		FTabManager::NewPrimaryArea()
		->SetOrientation(Orient_Vertical)
		->Split
		(
			FTabManager::NewStack()
			->SetSizeCoefficient(0.1f)
			->SetHideTabWell(true)
			->AddTab(GetToolbarTabId(),ETabState::OpenedTab)
		)
		->Split
		(
			FTabManager::NewSplitter()
			->SetOrientation(Orient_Horizontal)
			->SetSizeCoefficient(0.9f)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.5f)
				->SetHideTabWell(false)
				->AddTab(FCodeEditorTAB::TAB_Header,ETabState::OpenedTab)
				->AddTab(FCodeEditorTAB::TAB_Types,ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewStack()
				->SetSizeCoefficient(0.5f)
				->SetHideTabWell(false)
				->AddTab(FCodeEditorTAB::TAB_Script,ETabState::OpenedTab)
			)
			->Split
			(
				FTabManager::NewSplitter()
				->SetOrientation(Orient_Vertical)
				->SetSizeCoefficient(0.2f)
				->Split
				(
					FTabManager::NewStack()
					->SetSizeCoefficient(0.75f)
					->SetHideTabWell(true)
					->AddTab(FCodeEditorTAB::TAB_Details,ETabState::OpenedTab)
				)
			)
		)
	);
	//
	//
	// Initialize the asset editor and spawn the layout above
	InitAssetEditor(Mode,InitToolkitHost,MGC_APP,FCodeEditorLayout,/*bCreateDefaultStandaloneMenu=*/false,/*bCreateDefaultToolbar=*/true,ScriptObject);
	//
	// Extend things
	//ExtendMenu();
	//ExtendToolbar();
	//RegenerateMenusAndToolbars();
}

void FMGC_Toolkit::SET_MGC_Inline(UMagicNodeScript* NewScriptObject) {
	if ((NewScriptObject!=ScriptObject_Inline)&&(NewScriptObject!=nullptr)) {
		UMagicNodeScript* OldScriptObject = ScriptObject_Inline;
		ScriptObject_Inline = NewScriptObject;
		//
		RemoveEditingObject(OldScriptObject);
		AddEditingObject(NewScriptObject);
	}///
}

UMagicNodeScript* FMGC_Toolkit::GET_MGC_Inline() const {
	return ScriptObject_Inline;
}

TSharedRef<SDockTab> FMGC_Toolkit::TABSpawn_Script(const FSpawnTabArgs &Args) {
	const auto Label = FText(LOCTEXT("MGC_Script.Watermark","CPP"));
	//
	//
	SAssignNew(MGC_CodeEditor,SMGC_CodeEditorCore,ScriptObject_Inline)
	.SourceToEdit(EMGC_CodeSource::Script);
	//
	//
	return SNew(SDockTab)
	.Icon(FMGC_CodeEditorStyle::Get().Get()->GetBrush("MGC.Toolbar.CPP"))
	.Label(LOCTEXT("MGC_ScriptTitle","Script"))
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			MGC_CodeEditor.ToSharedRef()
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(15.f,15.f,15.f,15.f)
		[
			SNew(STextBlock).Text(Label)
			.Visibility(EVisibility::HitTestInvisible)
			.TextStyle(FEditorStyle::Get(),"Graph.CornerText")
		]
	];
}

TSharedRef<SDockTab> FMGC_Toolkit::TABSpawn_Header(const FSpawnTabArgs &Args) {
	const auto Label = FText(LOCTEXT("MGC_Header.Watermark","H"));
	//
	//
	SAssignNew(MGC_CodeEditor,SMGC_CodeEditorCore,ScriptObject_Inline)
	.SourceToEdit(EMGC_CodeSource::Header);
	//
	//
	return SNew(SDockTab)
	.Icon(FMGC_CodeEditorStyle::Get().Get()->GetBrush("MGC.Toolbar.H"))
	.Label(LOCTEXT("MGC_HeaderTitle","Header"))
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			MGC_CodeEditor.ToSharedRef()
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(15.f,15.f,15.f,15.f)
		[
			SNew(STextBlock).Text(Label)
			.Visibility(EVisibility::HitTestInvisible)
			.TextStyle(FEditorStyle::Get(),"Graph.CornerText")
		]
	];
}

TSharedRef<SDockTab> FMGC_Toolkit::TABSpawn_Types(const FSpawnTabArgs &Args) {
	const auto Label = FText(LOCTEXT("MGC_Types.Watermark","T"));
	//
	//
	SAssignNew(MGC_CodeEditor,SMGC_CodeEditorCore,ScriptObject_Inline)
	.SourceToEdit(EMGC_CodeSource::Types);
	//
	//
	return SNew(SDockTab)
	.Icon(FMGC_CodeEditorStyle::Get().Get()->GetBrush("MGC.Toolbar.T"))
	.Label(LOCTEXT("MGC_TypesTitle","Types"))
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			MGC_CodeEditor.ToSharedRef()
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(15.f,15.f,15.f,15.f)
		[
			SNew(STextBlock).Text(Label)
			.Visibility(EVisibility::HitTestInvisible)
			.TextStyle(FEditorStyle::Get(),"Graph.CornerText")
		]
	];
}

TSharedRef<SDockTab> FMGC_Toolkit::TABSpawn_Details(const FSpawnTabArgs &Args) {
	TSharedPtr<FMGC_Toolkit>FCodeEditor = SharedThis(this);
	//
	return SNew(SDockTab)
	.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
	.Label(LOCTEXT("MGC_DetailsTitle","Details"))
	[
		SNew(SMGC_PropertyTab,FCodeEditor)
	];
}

void FMGC_Toolkit::RegisterTabSpawners(const TSharedRef<FTabManager>&TBManager) {
	WorkspaceMenuCategory = TBManager->AddLocalWorkspaceMenuCategory(LOCTEXT("MGC_MagicNode_Workspace","MGC Code Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();
	FAssetEditorToolkit::RegisterTabSpawners(TBManager);
	//
	TBManager->RegisterTabSpawner(FCodeEditorTAB::TAB_Details,FOnSpawnTab::CreateSP(this,&FMGC_Toolkit::TABSpawn_Details))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(),"LevelEditor.Tabs.Viewports"))
	.SetDisplayName(LOCTEXT("MGC_Details_TabName","Details"))
	.SetGroup(WorkspaceMenuCategoryRef);
	//
	TBManager->RegisterTabSpawner(FCodeEditorTAB::TAB_Script,FOnSpawnTab::CreateSP(this,&FMGC_Toolkit::TABSpawn_Script))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(),"LevelEditor.Tabs.Viewports"))
	.SetDisplayName(LOCTEXT("MGC_Script_TabName","Script"))
	.SetGroup(WorkspaceMenuCategoryRef);
	//
	TBManager->RegisterTabSpawner(FCodeEditorTAB::TAB_Header,FOnSpawnTab::CreateSP(this,&FMGC_Toolkit::TABSpawn_Header))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(),"LevelEditor.Tabs.Viewports"))
	.SetDisplayName(LOCTEXT("MGC_Header_TabName","Header"))
	.SetGroup(WorkspaceMenuCategoryRef);
	//
	TBManager->RegisterTabSpawner(FCodeEditorTAB::TAB_Types,FOnSpawnTab::CreateSP(this,&FMGC_Toolkit::TABSpawn_Types))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(),"LevelEditor.Tabs.Viewports"))
	.SetDisplayName(LOCTEXT("MGC_Types_TabName","Types"))
	.SetGroup(WorkspaceMenuCategoryRef);
}

void FMGC_Toolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>&TBManager) {
	FAssetEditorToolkit::UnregisterTabSpawners(TBManager);
	//
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Details);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Script);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Header);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Types);
}

void FMGC_Toolkit::AddReferencedObjects(FReferenceCollector &Collector) {
	Collector.AddReferencedObject(ScriptObject_Inline);
}

FName FMGC_Toolkit::GetToolkitFName() const {
	return FName("MGC_CodeEditor");
}

FText FMGC_Toolkit::GetBaseToolkitName() const {
	return LOCTEXT("MGC_CodeEditor.Label","Magic Node Editor");
}

FText FMGC_Toolkit::GetToolkitName() const {
	const bool DirtyState = ScriptObject_Inline->GetOutermost()->IsDirty();
	//
	FFormatNamedArguments Args;
	Args.Add(TEXT("FsmName"),FText::FromString(ScriptObject_Inline->GetName()));
	Args.Add(TEXT("DirtyState"),(DirtyState)?FText::FromString(TEXT("*")):FText::GetEmpty());
	//
	return FText::Format(LOCTEXT("MGC_CodeEditor.Label","{FsmName}{DirtyState}"),Args);
}

FText FMGC_Toolkit::GetToolkitToolTipText() const {
	return GetToolTipTextForObject(ScriptObject_Inline);
}

FString FMGC_Toolkit::GetWorldCentricTabPrefix() const {
	return TEXT("MGC_CodeEditor");
}

FString FMGC_Toolkit::GetDocumentationLink() const {
	return TEXT("https://www.google.com/");
}

void FMGC_Toolkit::OnToolkitHostingStarted(const TSharedRef<IToolkit>&Toolkit) {
	/*TSharedPtr<SWidget>InlineContent = Toolkit->GetInlineContent();
	if (InlineContent.IsValid()) {
		??->SetContent(InlineContent.ToSharedRef());
	}*/
}

void FMGC_Toolkit::OnToolkitHostingFinished(const TSharedRef<IToolkit>&Toolkit) {
	//??->SetContent(SNullWidget::NullWidget);
}

FLinearColor FMGC_Toolkit::GetWorldCentricTabColorScale() const {
	return FLinearColor::White;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////