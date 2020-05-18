//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_Toolkit.h"

#include "MGC_EditorStyle.h"
#include "KMGC_KismetTypes.h"
#include "KMGC_ScriptParser.h"

#include "MagicNodeEditor_Shared.h"

#include "Editor/KismetWidgets/Public/SSingleObjectDetailsPanel.h"

#include "Editor/UnrealEd/Public/SourceCodeNavigation.h"
#include "Editor/LevelEditor/Public/LevelEditorActions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Toolkit Constructors:

FDelegateHandle FMGC_Toolkit::WatcherHandle = FDelegateHandle();

FMGC_Toolkit::FMGC_Toolkit() {
	static FDirectoryWatcherModule &DirectoryWatcherModule = FModuleManager::LoadModuleChecked<FDirectoryWatcherModule>(TEXT("DirectoryWatcher"));
	static FString ProjectRoot = FPaths::GameSourceDir();
	//
	DirectoryWatcherModule.Get()->RegisterDirectoryChangedCallback_Handle(
		ProjectRoot,IDirectoryWatcher::FDirectoryChanged::CreateRaw(
			this,&FMGC_Toolkit::OnProjectDirectoryChanged
		), FMGC_Toolkit::WatcherHandle, IDirectoryWatcher::WatchOptions::IncludeDirectoryChanges
	);//
	//
	//
	if (FMGC_Toolkit::SourceViewCount()==0) {
		RefreshEngineSourceView();
		RefreshPluginSourceView();
		RefreshProjectSourceView();
	}///
	//
	Search.Reset();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Details Native API:

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
		//
		SSingleObjectDetailsPanel::Construct(
			SSingleObjectDetailsPanel::FArguments().HostCommandList(MGC_Toolkit->GetToolkitCommands()).HostTabManager(MGC_Toolkit->GetTabManager()),
			true,
			true
		);//
	}///
	//
	virtual UObject* GetObjectToObserve() const override {
		return CodeEditor.Pin()->GET();
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
/// MGC Toolkit API:

void FMGC_Toolkit::INIT(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>&InitToolkitHost, UMagicNodeScript* ScriptObject) {
	GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->CloseOtherEditors(ScriptObject,this);
	//
	ScriptObject_Inline = ScriptObject;
	check(ScriptObject_Inline);
	//
	ScriptObject_Inline->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);
	//
	//
	const TSharedRef<FTabManager::FLayout>FCodeEditorLayout = FTabManager::NewLayout("MGC_CodeEditorLayout_V0022")
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
				->SetSizeCoefficient(0.15f)
				->SetHideTabWell(false)
				->AddTab(FCodeEditorTAB::TAB_TreeView,ETabState::OpenedTab)
			)
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
	);//
	//
	//
	InitAssetEditor(Mode,InitToolkitHost,MGC_APP,FCodeEditorLayout,true,true,ScriptObject);
	//
	BindCommands();
	ExtendMenu();
	ExtendToolbar();
	RegenerateMenusAndToolbars();
	//
	//
	if (SourceTreeWidget.IsValid()) {
		for (const TSharedPtr<FSourceTreeNode>&Node : GlobalSourceTreeView) {
			SourceTreeWidget->SetItemExpansion(Node,true);
		}///
	}///
	//
	if (MGC_CodeEditor.IsValid()) {
		MGC_CodeEditor->UpdateDatabaseReferences();
	}///
}

void FMGC_Toolkit::SET(UMagicNodeScript* NewScriptObject) {
	if ((NewScriptObject!=ScriptObject_Inline)&&(NewScriptObject!=nullptr)) {
		UMagicNodeScript* OldScriptObject = ScriptObject_Inline;
		ScriptObject_Inline = NewScriptObject;
		//
		RemoveEditingObject(OldScriptObject);
		AddEditingObject(NewScriptObject);
	}///
}

UMagicNodeScript* FMGC_Toolkit::GET() const {
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
	.Icon(FKMGC_NodeStyle::Get().Get()->GetBrush("KMGC.Toolbar.CPP"))
	.Label(LOCTEXT("MGC_ScriptTitle","Script"))
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			MGC_CodeEditor.ToSharedRef()
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(10.f,10.f,10.f,20.f)
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
	.Icon(FKMGC_NodeStyle::Get().Get()->GetBrush("KMGC.Toolbar.H"))
	.Label(LOCTEXT("MGC_HeaderTitle","Header"))
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			MGC_CodeEditor.ToSharedRef()
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(10.f,10.f,10.f,20.f)
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
	.Icon(FKMGC_NodeStyle::Get().Get()->GetBrush("KMGC.Toolbar.T"))
	.Label(LOCTEXT("MGC_TypesTitle","Types"))
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			MGC_CodeEditor.ToSharedRef()
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(10.f,10.f,10.f,20.f)
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

TSharedRef<SDockTab> FMGC_Toolkit::TABSpawn_TreeView(const FSpawnTabArgs &Args) {
	TSharedPtr<FMGC_Toolkit>FCodeEditor = SharedThis(this);
	Search = MakeShared<FString>(TEXT(""));
	//
	//
	return SNew(SDockTab)
	.Icon(FEditorStyle::GetBrush("LevelEditor.Tabs.Details"))
	.Label(LOCTEXT("MGC_TreeViewTitle","Source"))
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight().Padding(0,2,0,2)
		.VAlign(VAlign_Top).HAlign(HAlign_Fill)
		[
			SAssignNew(SourceViewSearchBox,SSearchBox)
			.OnTextCommitted(this,&FMGC_Toolkit::OnSearchCommitted)
			.OnTextChanged(this,&FMGC_Toolkit::OnSearchChanged)
			.SelectAllTextWhenFocused(true)
		]
		+SVerticalBox::Slot()
		.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
			[
				SNew(SBorder)
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				.Visibility(this,&FMGC_Toolkit::GetSourceTreeViewVisibility)
				[
					SAssignNew(SourceTreeWidget,STreeView<TSharedPtr<FSourceTreeNode>>)
					.OnMouseButtonDoubleClick(this,&FMGC_Toolkit::OnClickedSourceViewItem)
					.OnSelectionChanged(this,&FMGC_Toolkit::OnSelectedSourceViewItem)
					.OnExpansionChanged(this,&FMGC_Toolkit::OnExpansionChanged)
					.OnGenerateRow(this,&FMGC_Toolkit::OnGenerateSourceViewRow)
					.OnGetChildren(this,&FMGC_Toolkit::OnGetSourceViewChildren)
					.TreeItemsSource(&GlobalSourceTreeView)
					.SelectionMode(ESelectionMode::Single)
				]
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
			[
				SNew(SBorder)
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				.Visibility(this,&FMGC_Toolkit::GetSourceTreeSearchVisibility)
				[
					SAssignNew(SourceSearchWidget,STreeView<TSharedPtr<FSourceTreeNode>>)
					.OnMouseButtonDoubleClick(this,&FMGC_Toolkit::OnClickedSourceViewItem)
					.OnGenerateRow(this,&FMGC_Toolkit::OnGenerateSourceViewRow)
					.OnGetChildren(this,&FMGC_Toolkit::OnGetSourceViewChildren)
					.SelectionMode(ESelectionMode::Single)
					.TreeItemsSource(&SourceViewSearch)
				]
			]
		]
	];//
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<ITableRow>FMGC_Toolkit::OnGenerateSourceViewRow(TSharedPtr<FSourceTreeNode>InItem, const TSharedRef<STableViewBase>&OwnerTable) {
	FText Tooltip;
	//
	TSharedPtr<SImage>Icon = SNew(SImage)
	.Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.FolderClosed"));
	//
	if (InItem.IsValid()) {
		Tooltip = FText::FromString(InItem->FullPath);
		//
		if (InItem->Path.Len()>InItem->FullPath.Len()){
			Tooltip=FText::FromString(InItem->Path);
		}///
		//
		if (InItem->Path==TEXT("UNREAL")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.Engine"));
		}///
		//
		if (InItem->Path==TEXT("PLUGINS")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.Plugins"));
		}///
		//
		if (InItem->Path==TEXT("PROJECT")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.Project"));
		}///
		//
		if (InItem->Path.EndsWith(".h")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.HeaderFile"));
		}///
		//
		if (InItem->Path.EndsWith(".cpp")||InItem->Path.EndsWith(".hpp")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.CppFile"));
		}///
		//
		if (InItem->Path.EndsWith(".cs")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.CsFile"));
		}///
		//
		if (InItem->Path.EndsWith(".txt")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.Text"));
		}///
		//
		if (InItem->Path.EndsWith(".obj")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.Object"));
		}///
		//
		if (InItem->Path.EndsWith(".ini")||InItem->Path.EndsWith(".uproject")||InItem->Path.EndsWith(".uplugin")) {
			Icon = SNew(SImage).Image(FMagicNodeEditorStyle::Get()->GetBrush("SourceView.ConfigFile"));
		}///
	}///
	//
	//
	return SNew(STableRow<TSharedPtr<FSourceTreeNode>>,OwnerTable)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.Padding(1.f,0,1.f,0.f)
		.HAlign(HAlign_Left)
		.AutoWidth()
		[
			SNew(SBorder)
			.Padding(FMargin(0.f))
			.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
			.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
			[
				Icon.ToSharedRef()
			]
		]
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Fill)
		[
			SNew(SBorder)
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
			[
				SNew(STextBlock)
				.Text(FText::FromString(InItem->Path))
				.ToolTip(FSlateApplication::Get().MakeToolTip(Tooltip))
			]
		]
	];//
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::ExtendMenu() {
	////... @ToDo
}

void FMGC_Toolkit::BindCommands() {
	const FMagicNodeEditorCommands &Commands = FMagicNodeEditorCommands::Get();
	const TSharedRef<FUICommandList>&UICommandList = GetToolkitCommands();
	{
		UICommandList->MapAction(Commands.OpenSourceCodeViewer,FExecuteAction::CreateSP(this,&FMGC_Toolkit::LaunchSourceCodeViewer));
		UICommandList->MapAction(Commands.OpenSourceCodeSearch,FExecuteAction::CreateSP(this,&FMGC_Toolkit::LaunchSourceCodeSearch));
		UICommandList->MapAction(Commands.BuildDatabase,FExecuteAction::CreateSP(this,&FMGC_Toolkit::RebuildDatabases));
		//
		UICommandList->MapAction(
			Commands.Compile,
			FExecuteAction::CreateSP(this,&FMGC_Toolkit::CompileScript),
			FCanExecuteAction::CreateStatic(&FLevelEditorActionCallbacks::Recompile_CanExecute)
		);///
	}
}

void FMGC_Toolkit::ExtendToolbar() {
	struct Local {
		static void FillToolbar(FToolBarBuilder &ToolbarBuilder) {
			ToolbarBuilder.BeginSection("Command");
			{
				ToolbarBuilder.AddToolBarButton(FMagicNodeEditorCommands::Get().OpenSourceCodeViewer);
				ToolbarBuilder.AddToolBarButton(FMagicNodeEditorCommands::Get().OpenSourceCodeSearch);
				ToolbarBuilder.AddToolBarButton(FMagicNodeEditorCommands::Get().BuildDatabase);
			}
			ToolbarBuilder.EndSection();
			//
			if (FSourceCodeNavigation::IsCompilerAvailable()) {
				ToolbarBuilder.BeginSection("Compile");
				{
					ToolbarBuilder.AddToolBarButton(FMagicNodeEditorCommands::Get().Compile);
				}
				ToolbarBuilder.EndSection();
			}///
		}///
	};//
	//
	TSharedPtr<FExtender>ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension(
		"Asset", EExtensionHook::After, GetToolkitCommands(),
		FToolBarExtensionDelegate::CreateStatic(&Local::FillToolbar)
	);//
	//
	//
	AddToolbarExtender(ToolbarExtender);
 	IMagicNodeEditor* EditorModule = &FModuleManager::LoadModuleChecked<IMagicNodeEditor>("MagicNodeEditor");
 	AddToolbarExtender(EditorModule->GetMagicNodeEditorToolBarExtensibilityManager()->GetAllExtenders());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::LaunchSourceCodeViewer() {
	FMagicNodeEditor::InvokeSourceCodeViewerTAB(FTextLocation());
}

void FMGC_Toolkit::LaunchSourceCodeSearch() {
	FMagicNodeEditor::InvokeSourceCodeSearchTAB();
}

void FMGC_Toolkit::RebuildDatabases() {
	if (MGC_CodeEditor.IsValid()) {
		MGC_CodeEditor->UpdateDatabaseSemantics();
	}///
}

void FMGC_Toolkit::CompileProject() {
	if (FSourceCodeNavigation::IsCompilerAvailable()) {
		FLevelEditorActionCallbacks::RecompileGameCode_Clicked();
	}///
}

void FMGC_Toolkit::CompileScript() {
	if (GET()==nullptr) {return;}
	//
	EMGC_CompilerResult CompilerResult = CompileScriptClass(
		GET()->GetName(),
		GET()->Source.Header,
		GET()->Source.Script,
		GET()->Source.Types,
		GetParentClass(),
		GET()->Source.Includes,
		GET()->Source.Macros
	);///
	//
	if (CompilerResult != EMGC_CompilerResult::None) {
		switch (CompilerResult) {
			case EMGC_CompilerResult::Compiled:
			{
				LOG_MGC(EMGCSeverity::Info,Message[(uint32)CompilerResult]);
				//
				if (GEditor) {
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
				} CompileProject();
			} break;
			case EMGC_CompilerResult::ParsingFailure:
			{
				LOG_MGC(EMGCSeverity::Warning,Message[(uint32)CompilerResult]);
				//
				if (GEditor) {
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
				} CompileProject();
			} break;
			default:
			{
				LOG_MGC(EMGCSeverity::Error,Message[(uint32)CompilerResult]);
				//
				if (GEditor) {
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
				}///
			}///
		break;}
	}///
	//
	UpdateDatabaseReferences();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVisibility FMGC_Toolkit::GetSourceTreeViewVisibility() const {
	if (Search.IsValid()&&(Search->Len()<2)) {
		return EVisibility::Visible;
	}///
	//
	return EVisibility::Collapsed;
}

EVisibility FMGC_Toolkit::GetSourceTreeSearchVisibility() const {
	if (Search.IsValid()&&(Search->Len()<2)) {
		return EVisibility::Collapsed;
	}///
	//
	return EVisibility::Visible;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::OnClickedSourceViewItem(TSharedPtr<FSourceTreeNode>TreeItem) {
	if (!TreeItem.IsValid()) {return;}
	//
	if (IsSourceFile(TreeItem->FullPath)) {
		ViewerSourcePath = TreeItem->FullPath;
		LaunchSourceCodeViewer();
	} else {SourceTreeWidget->SetItemExpansion(TreeItem,true);}
}

void FMGC_Toolkit::OnSelectedSourceViewItem(TSharedPtr<FSourceTreeNode>TreeItem, ESelectInfo::Type SelectInfo) {
	if (!TreeItem.IsValid()) {return;}
	//
	if (IsSourceFile(TreeItem->FullPath)) {
		ViewerSourcePath = TreeItem->FullPath;
	}///
}

void FMGC_Toolkit::OnGetSourceViewChildren(TSharedPtr<FSourceTreeNode>InItem, TArray<TSharedPtr<FSourceTreeNode>>&OutChildren) {
	if (SourceViewSearch.Num()==0) {OutChildren=InItem->ChildNodes;}
}

void FMGC_Toolkit::OnExpansionChanged(TSharedPtr<FSourceTreeNode>InItem, bool WasExpanded){}
void FMGC_Toolkit::OnSourceViewCheckStatusChanged(ECheckBoxState NewCheckState, TSharedPtr<FSourceTreeNode>NodeChanged){}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::RefreshEngineSourceView() {
	static FString SourceRoot = FPaths::EngineSourceDir();
	static TArray<FString>EngineSource;
	//
	IFileManager &FileManager = IFileManager::Get();
	FileManager.FindFilesRecursive(EngineSource,*SourceRoot,TEXT("/*.*"),true,false);
	if (EngineSource.Num()==0) {return;}
	//
	FString RootPath; 
	FString FullPath = FPaths::ConvertRelativePathToFull(EngineSource[0]);
	//
	FullPath.Split(TEXT("/Engine/Source/"),&RootPath,nullptr);
	RootPath += TEXT("/Engine/Source/");
	//
	TSharedPtr<FSourceTreeNode>RootNode = MakeShared<FSourceTreeNode>();
	RootNode->Path = TEXT("UNREAL"); RootNode->FullPath = RootPath;
	//
	TSharedRef<FSourceTreeNode>OldRoot = RootNode.ToSharedRef();
	for (const TSharedPtr<FSourceTreeNode>&Old : GlobalSourceTreeView) {
		if (Old->Path==RootNode->Path) {OldRoot=Old.ToSharedRef(); break;}
	} GlobalSourceTreeView.Remove(OldRoot);
	//
	GlobalSourceTreeView.Add(RootNode);
	TSharedRef<FSourceTreeNode>ParentNode = RootNode.ToSharedRef();
	//
	for (FString &Path : EngineSource) {
		Path = FPaths::ConvertRelativePathToFull(Path);
		Path.ReplaceInline(*RootPath,TEXT(""));
	}///
	//
	for (const FString &Path : EngineSource) {
		if (Path.Contains(TEXT("/Intermediate/"))) {continue;}
		if (!IsSourceFile(Path)) {continue;}
		if (Path.IsEmpty()) {continue;}
		//
		TArray<FString>Nodes;
		Path.ParseIntoArray(Nodes,TEXT("/"));
		FString Source = ParentNode->FullPath;
		//
		for (int32 I=0; I<Nodes.Num(); I++) {
			const FString &Node = Nodes[I];
			Source = FPaths::Combine(Source,Node);
			TSharedPtr<FSourceTreeNode>TreeNode = ParentNode->FindNode(Node);
			//
			if (!TreeNode.IsValid()) {
				TreeNode = MakeShared<FSourceTreeNode>();
				TreeNode->ParentNode = ParentNode;
				TreeNode->FullPath = Source;
				TreeNode->Path = Node;
				//
				ParentNode->ChildNodes.Add(TreeNode);
			} ParentNode = TreeNode.ToSharedRef();
		}///
		//
		ParentNode = RootNode.ToSharedRef();
	}///
}

void FMGC_Toolkit::RefreshPluginSourceView() {
	static FString PluginRoot = FPaths::ProjectPluginsDir();
	static TArray<FString>PluginSource;
	//
	IFileManager &FileManager = IFileManager::Get();
	FileManager.FindFilesRecursive(PluginSource,*PluginRoot,TEXT("/*.*"),true,false);
	if (PluginSource.Num()==0) {return;}
	//
	FString RootPath; 
	FString FullPath = FPaths::ConvertRelativePathToFull(PluginSource[0]);
	//
	FullPath.Split(TEXT("/Plugins/"),&RootPath,nullptr);
	RootPath += TEXT("/Plugins/");
	//
	TSharedPtr<FSourceTreeNode>RootNode = MakeShared<FSourceTreeNode>();
	RootNode->Path = TEXT("PLUGINS"); RootNode->FullPath = RootPath;
	//
	TSharedRef<FSourceTreeNode>OldRoot = RootNode.ToSharedRef();
	for (const TSharedPtr<FSourceTreeNode>&Old : GlobalSourceTreeView) {
		if (Old->Path==RootNode->Path) {OldRoot=Old.ToSharedRef(); break;}
	} GlobalSourceTreeView.Remove(OldRoot);
	//
	GlobalSourceTreeView.Add(RootNode);
	TSharedRef<FSourceTreeNode>ParentNode = RootNode.ToSharedRef();
	//
	for (FString &Path : PluginSource) {
		Path = FPaths::ConvertRelativePathToFull(Path);
		Path.ReplaceInline(*RootPath,TEXT(""));
	}///
	//
	for (const FString &Path : PluginSource) {
		if (!IsSourceFile(Path)) {continue;}
		if (Path.IsEmpty()) {continue;}
		//
		TArray<FString>Nodes;
		Path.ParseIntoArray(Nodes,TEXT("/"));
		FString Source = ParentNode->FullPath;
		//
		for (int32 I=0; I<Nodes.Num(); I++) {
			const FString &Node = Nodes[I];
			Source = FPaths::Combine(Source,Node);
			TSharedPtr<FSourceTreeNode>TreeNode = ParentNode->FindNode(Node);
			//
			if (!TreeNode.IsValid()) {
				TreeNode = MakeShared<FSourceTreeNode>();
				TreeNode->ParentNode = ParentNode;
				TreeNode->FullPath = Source;
				TreeNode->Path = Node;
				//
				ParentNode->ChildNodes.Add(TreeNode);
			} ParentNode = TreeNode.ToSharedRef();
		}///
		//
		ParentNode = RootNode.ToSharedRef();
	}///
}

void FMGC_Toolkit::RefreshProjectSourceView() {
	static FString ProjectRoot = FPaths::GameSourceDir();
	static TArray<FString>ProjectSource;
	//
	IFileManager &FileManager = IFileManager::Get();
	FileManager.FindFilesRecursive(ProjectSource,*ProjectRoot,TEXT("/*.*"),true,false);
	if (ProjectSource.Num()==0) {return;}
	//
	FString RootPath; 
	FString FullPath = FPaths::ConvertRelativePathToFull(ProjectSource[0]);
	//
	FullPath.Split(TEXT("/Source/"),&RootPath,nullptr);
	RootPath += TEXT("/Source/");
	//
	TSharedPtr<FSourceTreeNode>RootNode = MakeShared<FSourceTreeNode>();
	RootNode->Path = TEXT("PROJECT"); RootNode->FullPath = RootPath;
	//
	TSharedRef<FSourceTreeNode>OldRoot = RootNode.ToSharedRef();
	for (const TSharedPtr<FSourceTreeNode>&Old : GlobalSourceTreeView) {
		if (Old->Path==RootNode->Path) {OldRoot=Old.ToSharedRef(); break;}
	} GlobalSourceTreeView.Remove(OldRoot);
	//
	GlobalSourceTreeView.Add(RootNode);
	TSharedRef<FSourceTreeNode>ParentNode = RootNode.ToSharedRef();
	//
	for (FString &Path : ProjectSource) {
		Path = FPaths::ConvertRelativePathToFull(Path);
		Path.ReplaceInline(*RootPath,TEXT(""));
	}///
	//
	for (const FString &Path : ProjectSource) {
		if (!IsSourceFile(Path)) {continue;}
		if (Path.IsEmpty()) {continue;}
		//
		TArray<FString>Nodes;
		Path.ParseIntoArray(Nodes,TEXT("/"));
		FString Source = ParentNode->FullPath;
		//
		for (int32 I=0; I<Nodes.Num(); I++) {
			const FString &Node = Nodes[I];
			Source = FPaths::Combine(Source,Node);
			TSharedPtr<FSourceTreeNode>TreeNode = ParentNode->FindNode(Node);
			//
			if (!TreeNode.IsValid()) {
				TreeNode = MakeShared<FSourceTreeNode>();
				TreeNode->ParentNode = ParentNode;
				TreeNode->FullPath = Source;
				TreeNode->Path = Node;
				//
				ParentNode->ChildNodes.Add(TreeNode);
			} ParentNode = TreeNode.ToSharedRef();
		}///
		//
		ParentNode = RootNode.ToSharedRef();
	}///
}

void FMGC_Toolkit::UpdateDatabaseReferences() {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {return;}
	if (_Settings->SemanticDB.Num()==0) {return;}
	//
	UMGC_SemanticDB* SemanticDB = _Settings->SemanticDB.Array()[0].Get();
	UMagicNodeScript* Script = GET();
	//
	if (SemanticDB==nullptr) {return;}
	if (Script==nullptr) {return;}
	//
	if (Script->RefreshRuntimeScriptClass()) {
		SemanticDB->RegisterClassReflection(Script->GetRuntimeScriptClass(),TEXT("U"));
		//
		FString Name=Script->GetName();
		Name.RemoveFromEnd(TEXT("_C"));
		Name.RemoveFromStart(TEXT("Default__"));
		//
		FClassRedirector CR;
		CR.ObjectName = Name;
		CR.ObjectClass = FString(TEXT("U"))+(Script->GetRuntimeScriptClass()->GetName());
		SemanticDB->ClassRedirectors.Add(CR.ObjectName,CR);
		//
		FString Class = FString::Printf(TEXT("U%s"),*Script->GetRuntimeScriptClass()->GetName());
		IKMGC_ScriptParser::ParseClassFromHeader(Script->Source.Header,Class);
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::RegisterTabSpawners(const TSharedRef<FTabManager>&TBManager) {
	WorkspaceMenuCategory = TBManager->AddLocalWorkspaceMenuCategory(LOCTEXT("MGC_MagicNode_Workspace","Code Editor"));
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
	//
	TBManager->RegisterTabSpawner(FCodeEditorTAB::TAB_TreeView,FOnSpawnTab::CreateSP(this,&FMGC_Toolkit::TABSpawn_TreeView))
	.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(),"LevelEditor.Tabs.Viewports"))
	.SetDisplayName(LOCTEXT("MGC_Types_TabName","Source"))
	.SetGroup(WorkspaceMenuCategoryRef);
}

void FMGC_Toolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>&TBManager) {
	FAssetEditorToolkit::UnregisterTabSpawners(TBManager);
	//
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_TreeView);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Details);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Script);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Header);
	TBManager->UnregisterTabSpawner(FCodeEditorTAB::TAB_Types);
}

void FMGC_Toolkit::AddReferencedObjects(FReferenceCollector &Collector) {
	Collector.AddReferencedObject(ScriptObject_Inline);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	Args.Add(TEXT("MgcName"),FText::FromString(ScriptObject_Inline->GetName()));
	Args.Add(TEXT("DirtyState"),(DirtyState)?FText::FromString(TEXT("*")):FText::GetEmpty());
	//
	return FText::Format(LOCTEXT("MGC_CodeEditor.Label","{MgcName}{DirtyState}"),Args);
}

FText FMGC_Toolkit::GetToolkitToolTipText() const {
	return GetToolTipTextForObject(ScriptObject_Inline);
}

FString FMGC_Toolkit::GetWorldCentricTabPrefix() const {
	return TEXT("MGC_CodeEditor");
}

FString FMGC_Toolkit::GetDocumentationLink() const {
	return TEXT("https://brunoxavierleite.wordpress.com/2019/01/16/unreal-magic-nodes-programming/");
}

FLinearColor FMGC_Toolkit::GetWorldCentricTabColorScale() const {
	return FLinearColor::White;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FMGC_Toolkit::GetParentClass() const {
	if (GET()==nullptr) {return FString();}
	//
	FString Name = GET()->ParentClass->GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	//
	return Name;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::OnSearchChanged(const FText &Filter) {
	Search = MakeShared<FString>(Filter.ToString());
	SourceViewSearch.Empty();
	//
	for (const TSharedPtr<FSourceTreeNode>&Node : GlobalSourceTreeView) {
		if (Node->Path.Contains(**Search.Get())) {
			SourceViewSearch.Add(Node);
		}///
		//
		for (const TSharedPtr<FSourceTreeNode>&N1 : Node->ChildNodes) {
			if (N1->Path.Contains(**Search.Get())) {
				SourceViewSearch.Add(N1);
			}///
			//
			for (const TSharedPtr<FSourceTreeNode>&N2 : N1->ChildNodes) {
				if (N2->Path.Contains(**Search.Get())) {
					SourceViewSearch.Add(N2);
				}///
				//
				for (const TSharedPtr<FSourceTreeNode>&N3 : N2->ChildNodes) {
					if (N3->Path.Contains(**Search.Get())) {
						SourceViewSearch.Add(N3);
					}///
					//
					for (const TSharedPtr<FSourceTreeNode>&N4 : N3->ChildNodes) {
						if (N4->Path.Contains(**Search.Get())) {
							SourceViewSearch.Add(N4);
						}///
						//
						for (const TSharedPtr<FSourceTreeNode>&N5 : N4->ChildNodes) {
							if (N5->Path.Contains(**Search.Get())) {
								SourceViewSearch.Add(N5);
							}///
							//
							for (const TSharedPtr<FSourceTreeNode>&N6 : N5->ChildNodes) {
								if (N6->Path.Contains(**Search.Get())) {
									SourceViewSearch.Add(N6);
								}///
							}///
						}///
					}///
				}///
			}///
		}///
	}///
	//
	//
	SourceSearchWidget->RequestListRefresh();
}

void FMGC_Toolkit::OnSearchCommitted(const FText &NewText, ETextCommit::Type CommitInfo) {
	if (NewText.ToString().Len()<2) {
		SourceViewSearch.Empty();
	}///
	//
	SourceSearchWidget->RequestListRefresh();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_Toolkit::OnProjectDirectoryChanged(const TArray<FFileChangeData> &Data) {
	RefreshPluginSourceView();
	RefreshProjectSourceView();
}

void FMGC_Toolkit::OnToolkitHostingStarted(const TSharedRef<IToolkit>&Toolkit) {}
void FMGC_Toolkit::OnToolkitHostingFinished(const TSharedRef<IToolkit>&Toolkit){}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool FMGC_Toolkit::IsSourceFile(const FString &Path) {
	return (
		Path.EndsWith(TEXT(".h"))||Path.EndsWith(TEXT(".cpp"))||Path.EndsWith(TEXT(".txt")) ||
		Path.EndsWith(TEXT(".c"))||Path.EndsWith(TEXT(".cs"))||Path.EndsWith(TEXT(".hpp"))  ||
		Path.EndsWith(TEXT(".uproject"))||Path.EndsWith(TEXT(".uplugin"))||Path.EndsWith(TEXT(".ini"))
	);//
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////