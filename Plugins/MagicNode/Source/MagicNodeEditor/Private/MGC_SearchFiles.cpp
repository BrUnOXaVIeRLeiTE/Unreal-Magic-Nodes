//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_SearchFiles.h"

#include "MGC_Toolkit.h"
#include "MagicNodeEditor_Shared.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMGC_SearchFiles::SMGC_SearchFiles() {
	SelectedSearchRoot = TEXT("All");
	RequestListRefresh = false;
}

SMGC_SearchFiles::~SMGC_SearchFiles() {
	SearchState = ESearchState::COMPLETE;
	SearchResultList.Empty();
}

FAsyncTask<TASK_SearchFiles>* SMGC_SearchFiles::ASYNC_Search = nullptr;

FString TASK_SearchFiles::SearchText = FString();
FString TASK_SearchFiles::SearchRoot = FString();

bool TASK_SearchFiles::CaseSensitive = false;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_SearchFiles::Construct(const FArguments &InArgs) {
	SearchText = InArgs._SearchWord;
	//
	//
	SEARCH_SCROLL = SNew(SScrollBar)
	.OnUserScrolled(this,&SMGC_SearchFiles::OnInternalSearchResultScroll)
	.Thickness(FVector2D(8.f,8.f)).AlwaysShowScrollbar(false)
	.Orientation(Orient_Vertical);
	//
	SearchRoots.Reset();
	SearchRoots.Add(MakeShareable(new FString(TEXT("All"))));
	SearchRoots.Add(MakeShareable(new FString(TEXT("Engine"))));
	SearchRoots.Add(MakeShareable(new FString(TEXT("Plugins"))));
	SearchRoots.Add(MakeShareable(new FString(TEXT("Project"))));
	//
	//
	ChildSlot[
	SNew(SVerticalBox)
	+SVerticalBox::Slot().AutoHeight()
	.VAlign(VAlign_Top).HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot().Padding(2)
		.VAlign(VAlign_Top).HAlign(HAlign_Fill)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.AutoHeight().Padding(2,0,2,0)
			[
				SNew(STextBlock)
				.Margin(FMargin(2,0,2,0))
				.Text(FText::FromString("Search:  "))
				.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f)))
			]
			+SVerticalBox::Slot()
			.AutoHeight().Padding(2,0,2,0)
			[
				SAssignNew(SEARCH_TEXT,SEditableTextBox)
				.OnTextChanged(this,&SMGC_SearchFiles::OnSearchTextChanged,ETextCommit::Default)
				.OnTextCommitted(this,&SMGC_SearchFiles::OnSearchTextComitted)
				.Text(this,&SMGC_SearchFiles::GetSearchText)
				.IsEnabled(this,&SMGC_SearchFiles::CanEdit)
				.SelectAllTextWhenFocused(true)
			]
			+SVerticalBox::Slot()
			.AutoHeight().Padding(2,6,2,0)
			[
				SNew(STextBlock)
				.Margin(FMargin(2,0,2,0))
				.Text(FText::FromString("Look in:  "))
				.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f)))
			]
			+SVerticalBox::Slot()
			.AutoHeight().Padding(2,0,2,0)
			[
				SAssignNew(SEARCH_ROOTPATH,SComboBox<TSharedPtr<FString>>)
				.OnGenerateWidget(this,&SMGC_SearchFiles::OnGenerateRootSelectionWidget)
				.OnSelectionChanged(this,&SMGC_SearchFiles::OnSelectedRootChanged)
				.IsEnabled(this,&SMGC_SearchFiles::CanEdit)
				.OptionsSource(&SearchRoots)
				[
					SNew(STextBlock)
					.Text(this,&SMGC_SearchFiles::GetSelectedSearchRoot)
				]
			]
		]
		+SHorizontalBox::Slot()
		.Padding(2,9,2,0).AutoWidth()
		.VAlign(VAlign_Top).HAlign(HAlign_Left)
		[
			SNew(SCheckBox)
			.OnCheckStateChanged(this,&SMGC_SearchFiles::OnSearchSensitiveChanged)
			.IsChecked(this,&SMGC_SearchFiles::IsSearchSensitive)
			.Style(FEditorStyle::Get(),"ToggleButtonCheckbox")
			.IsEnabled(this,&SMGC_SearchFiles::CanEdit)
			.Content()
			[
				SNew(STextBlock).Margin(2)
				.Text(FText::FromString("Aa"))
				.Justification(ETextJustify::Center)
				.ColorAndOpacity(FSlateColor(FLinearColor(FColor(255,255,255,225))))
				.Font(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font)
			]
		]
	]
	+SVerticalBox::Slot()
	.Padding(2,4).AutoHeight()
	.VAlign(VAlign_Top).HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot().Padding(2,0,4,0)
		.VAlign(VAlign_Top).HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.IsEnabled(this,&SMGC_SearchFiles::CanSearch)
			.ForegroundColor(FSlateColor::UseForeground())
			.ButtonStyle(FEditorStyle::Get(),"FlatButton.Default")
			.OnClicked(this,&SMGC_SearchFiles::OnClickedSearchFiles)
			[
				SNew(STextBlock).Margin(2)
				.Text(FText::FromString("SEARCH"))
				.Justification(ETextJustify::Center)
				.Font(FEditorStyle::GetFontStyle(TEXT("BoldFont")))
			]
		]
		+SHorizontalBox::Slot().Padding(4,0,2,0)
		.VAlign(VAlign_Top).HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.HAlign(HAlign_Center)
			.IsEnabled(this,&SMGC_SearchFiles::CanCancel)
			.ForegroundColor(FSlateColor::UseForeground())
			.ButtonStyle(FEditorStyle::Get(),"FlatButton.Default")
			.OnClicked(this,&SMGC_SearchFiles::OnClickedCancelSearch)
			[
				SNew(STextBlock).Margin(2)
				.Text(FText::FromString("CANCEL"))
				.Justification(ETextJustify::Center)
				.Font(FEditorStyle::GetFontStyle(TEXT("BoldFont")))
			]
		]
	]
	+SVerticalBox::Slot()
	.Padding(2,2).AutoHeight()
	[
		SNew(SSeparator)
	]
	+SVerticalBox::Slot()
	.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
	[
		SNew(SBox)
		.MinDesiredHeight(250.f)
		[
			SNew(SBorder)
			.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
			.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
			[
				SAssignNew(SEARCH_SCROLLBOX,SScrollBox)
				.OnUserScrolled(this,&SMGC_SearchFiles::OnSearchResultScroll)
				.Orientation(EOrientation::Orient_Vertical)
				.ScrollBarThickness(FVector2D(8.f,8.f))
				//
				+SScrollBox::Slot()
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				[
					SAssignNew(SEARCH_RESULTS,SListView<TSharedPtr<FSearchInfo>>)
					.OnMouseButtonDoubleClick(this,&SMGC_SearchFiles::OnClickedSearchResult)
					.OnSelectionChanged(this,&SMGC_SearchFiles::OnSelectedSearchResult)
					.OnGenerateRow(this,&SMGC_SearchFiles::OnGenerateSearchResult)
					.SelectionMode(ESelectionMode::Single)
					.ListItemsSource(&SearchResultList)
					.ExternalScrollbar(SEARCH_SCROLL)
				]
			]
		]
	]
	+SVerticalBox::Slot()
	.AutoHeight().Padding(0)
	.VAlign(VAlign_Bottom).HAlign(HAlign_Fill)
	[
		SNew(SBorder)
		.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
		.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
		.Visibility(this,&SMGC_SearchFiles::GetSearchProgressVisibility)
		[
			SNew(SProgressBar)
			.BorderPadding(FVector2D::ZeroVector)
			.Percent(this,&SMGC_SearchFiles::GetSearchLoad)
			.FillColorAndOpacity(FSlateColor(FLinearColor(0.5f,0.5f,1.f)))
		]
	]
	];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_SearchFiles::Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) {
	if (CanCancel()) {
		if (SearchLoad>=1.f) {
			SearchLoad = 0.f;
		} else {SearchLoad+=0.05f;}
	} else if (RequestListRefresh) {
		if (ASYNC_Search && (ASYNC_Search->IsDone()||ASYNC_Search->IsIdle())) {
			SearchState = ESearchState::COMPLETE;
		}///
		//
		SearchLoad = 0.f;
		RequestListRefresh = false;
		//
		if (SEARCH_RESULTS.IsValid()) {SEARCH_RESULTS->RebuildList();}
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<ITableRow> SMGC_SearchFiles::OnGenerateSearchResult(TSharedPtr<FSearchInfo>Item, const TSharedRef<STableViewBase>&OwnerTable) {
	if (Item.IsValid()&&(TASK_SearchFiles::IsSourceFile(Item.Get()->MatchFilePath))) {
		TSharedRef<SImage>ICON = SNew(SImage).Image(FEditorStyle::GetBrush(TEXT("Kismet.VariableList.TypeIcon")));
		FSlateColor COLOR = FSlateColor(FLinearColor(0.0f,0.5f,1.f,1.f));
		//
		if (Item.Get()->MatchLine.StartsWith(TEXT("UFUNCTION"),ESearchCase::CaseSensitive)
		||	Item.Get()->MatchLine.StartsWith(TEXT("void"),ESearchCase::CaseSensitive)) {
			COLOR = FSlateColor(FLinearColor(1.f,0.3f,0.f,1.f));
			ICON = SNew(SImage).Image(FEditorStyle::GetBrush(TEXT("Kismet.AllClasses.FunctionIcon"))).ColorAndOpacity(COLOR);
		}///
		//
		if (Item.Get()->MatchLine.StartsWith(TEXT("class"),ESearchCase::CaseSensitive)
		||	Item.Get()->MatchLine.StartsWith(TEXT("UCLASS"),ESearchCase::CaseSensitive)
		||	Item.Get()->MatchLine.StartsWith(TEXT("public class"),ESearchCase::CaseSensitive)
		||	Item.Get()->MatchLine.StartsWith(TEXT("private class"),ESearchCase::CaseSensitive)
		||	Item.Get()->MatchLine.StartsWith(TEXT("protected class"),ESearchCase::CaseSensitive)) {
			COLOR = FSlateColor(FLinearColor(1.f,0.55f,0.95f,1.f));
			ICON = SNew(SImage).Image(FEditorStyle::GetBrush(TEXT("CodeView.ClassIcon")));
		}///
		//
		if (Item.Get()->MatchLine.StartsWith(TEXT("//"),ESearchCase::CaseSensitive)
		||	Item.Get()->MatchLine.StartsWith(TEXT("/*"),ESearchCase::CaseSensitive)) {
			COLOR = FSlateColor(FLinearColor(0.f,0.9f,0.1f,1.f));
		}///
		//
		if (Item.Get()->MatchLine.StartsWith("#",ESearchCase::CaseSensitive)) {
			COLOR = FSlateColor(FLinearColor(0.45f,0.45f,0.45f,1.f));
		}///
		//
		return SNew(SComboRow<TSharedRef<FString>>,OwnerTable)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth().VAlign(VAlign_Center).Padding(4,0,4,0)
			[
				ICON
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SBorder)
				.ColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f))
				.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
				[
					SNew(STextBlock)
					.ColorAndOpacity(COLOR)
					.Margin(FMargin(2.f,2.f,2.f,2.f))
					.Text(FText::FromString(Item.Get()->MatchLine))
				]
			]
		];//
	}///
	//
	return SNew(SComboRow<TSharedRef<FSearchInfo>>,OwnerTable);
}

TSharedRef<SWidget> SMGC_SearchFiles::OnGenerateRootSelectionWidget(TSharedPtr<FString>Item) {
	return SNew(STextBlock).Text(FText::FromString(*Item));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_SearchFiles::OnClickedSearchResult(TSharedPtr<FSearchInfo>Item) {
	if (Item.IsValid()) {
		ViewerSourcePath = Item->MatchFilePath;
		FMagicNodeEditor::InvokeSourceCodeViewerTAB(Item->MatchWordLocation);
	}///
}

void SMGC_SearchFiles::OnSelectedSearchResult(TSharedPtr<FSearchInfo>Item, ESelectInfo::Type SelectInfo) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_SearchFiles::OnSearchResultScroll(float Offset) {
	SEARCH_SCROLL->SetState(SEARCH_SCROLLBOX->GetScrollOffset(),SEARCH_SCROLLBOX->GetViewOffsetFraction());
}

void SMGC_SearchFiles::OnInternalSearchResultScroll(float Offset) {
	SEARCH_SCROLLBOX->SetScrollOffset(Offset);
}

void SMGC_SearchFiles::OnSelectedRootChanged(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo) {
	if (Item.IsValid()) {SelectedSearchRoot=(*Item.Get());}
}

void SMGC_SearchFiles::OnSearchSensitiveChanged(ECheckBoxState NewState) {
	SensitiveSearch = (NewState==ECheckBoxState::Checked);
}

void SMGC_SearchFiles::OnSearchTextChanged(const FText &InText, ETextCommit::Type CommitType) {
	SearchText = InText.ToString();
}

void SMGC_SearchFiles::OnSearchTextComitted(const FText &NewText, ETextCommit::Type CommitInfo) {
	SearchText.Empty(); SearchText.Append(NewText.ToString());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FReply SMGC_SearchFiles::OnClickedSearchFiles() {
	if (SearchText.IsEmpty()) {return FReply::Handled();}
	//
	TASK_SearchFiles::SearchText = SearchText;
	TASK_SearchFiles::CaseSensitive = SensitiveSearch;
	TASK_SearchFiles::SearchRoot = (SelectedSearchRoot==TEXT("Engine")) ? TEXT("UNREAL") : SelectedSearchRoot;
	//
	if (SMGC_SearchFiles::ASYNC_Search==nullptr) {
		SMGC_SearchFiles::ASYNC_Search = (new FAsyncTask<TASK_SearchFiles>());
	}///
	//
	if (ASYNC_Search->IsIdle()) {
		SMGC_SearchFiles::ASYNC_Search->StartBackgroundTask();
		SearchState = ESearchState::SEARCHING;
	}///
	//
	return FReply::Handled();
}

FReply SMGC_SearchFiles::OnClickedCancelSearch() {
	if (ASYNC_Search) {
		ASYNC_Search->EnsureCompletion(false);
		SearchState = ESearchState::COMPLETE;
	}///
	//
	return FReply::Handled();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EVisibility SMGC_SearchFiles::GetSearchProgressVisibility() const {
	if (CanCancel()) {return EVisibility::Visible;}
	//
	return EVisibility::Collapsed;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TOptional<float>SMGC_SearchFiles::GetSearchLoad() const {
	return TOptional<float>(SearchLoad);
}

FText SMGC_SearchFiles::GetSearchText() const {
	if (SearchText.IsEmpty()) {return FText::FromString(TEXT("Search.."));}
	//
	return FText::FromString(SearchText);;
}

FText SMGC_SearchFiles::GetSelectedSearchRoot() const {
	return FText::FromString(SelectedSearchRoot);
}

ECheckBoxState SMGC_SearchFiles::IsSearchSensitive() const {
	return (SensitiveSearch) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SMGC_SearchFiles::CanSearch() const {
	return ((!SearchText.IsEmpty())&&(SearchState==ESearchState::COMPLETE));
}

bool SMGC_SearchFiles::CanEdit() const {
	return (SearchState==ESearchState::COMPLETE);
}

bool SMGC_SearchFiles::CanCancel() const {
	return (
		(SearchState==ESearchState::SEARCHING) &&
		(ASYNC_Search && !ASYNC_Search->IsDone())
	);///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool TASK_SearchFiles::IsSourceFile(const FString &Path) {
	return (
		Path.EndsWith(TEXT(".h"))||Path.EndsWith(TEXT(".cpp"))||Path.EndsWith(TEXT(".txt")) ||
		Path.EndsWith(TEXT(".c"))||Path.EndsWith(TEXT(".cs"))||Path.EndsWith(TEXT(".hpp"))  ||
		Path.EndsWith(TEXT(".uproject"))||Path.EndsWith(TEXT(".uplugin"))||Path.EndsWith(TEXT(".ini"))
	);//
}

bool TASK_SearchFiles::SearchInFile(const FString &FilePath) {
	if (IsSourceFile(FilePath)) {
		FString SourceFile;
		//
		if (FFileHelper::LoadFileToString(SourceFile,*FilePath)) {
			TArray<FString>SourceLines;
			int32 LO = INDEX_NONE;
			int32 LI = 0;
			//
			bool AnyFound = false;
			SourceFile.ParseIntoArrayLines(SourceLines,false);
			//
			for (auto IT = SourceLines.CreateConstIterator(); IT; ++IT) {
				if (TASK_SearchFiles::CaseSensitive) {
					LO = IT->Find(TASK_SearchFiles::SearchText,ESearchCase::CaseSensitive);
				} else {LO=IT->Find(TASK_SearchFiles::SearchText,ESearchCase::IgnoreCase);}
				//
				if (LO>INDEX_NONE) {
					FTextLocation MatchLocation(LI,LO); AnyFound = true;
					SearchResultList.Add(MakeShareable(new FSearchInfo((*IT).TrimStartAndEnd(),FilePath,MatchLocation)));
				}///
			LI++;}
			//
			return AnyFound;
		} else {return false;}
	}///
	//
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////