//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_CodeEditorCore.h"

#include "KMGC_NodeStyle.h"
#include "KMGC_NodeWidget.h"
#include "KMGC_TextSyntaxHighlighter.h"

#include "Runtime/SlateCore/Public/Widgets/SOverlay.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SMGC_CodeEditorCore::SMGC_CodeEditorCore() {
	ExternalFilePath = MakeShareable(new FString());
	ExternalFileText = MakeShareable(new FString());
}

SMGC_CodeEditorCore::~SMGC_CodeEditorCore(){}

void SMGC_CodeEditorCore::Construct(const FArguments &InArgs, UMagicNodeScript* InScriptObject) {
	ExternalFilePath = MakeShareable(new FString(InArgs._ExternalSourcePath));
	Source = InArgs._SourceToEdit;
	//
	if (IsSourceView()) {
		(new FAutoDeleteAsyncTask<TASK_LoadFile>(ExternalFileText,ExternalFilePath))->StartBackgroundTask();
	} else {check(InScriptObject); ScriptObject=InScriptObject;}
	//
	UpdateDatabaseReferences();
	//
	//
	VS_SCROLL = SNew(SScrollBar)
	.OnUserScrolled(this,&SMGC_CodeEditorCore::OnInternalVerticalScroll)
	.Thickness(FVector2D(8.f,8.f)).AlwaysShowScrollbar(false)
	.Orientation(Orient_Vertical);
	//
	HS_SCROLL = SNew(SScrollBar)
	.OnUserScrolled(this,&SMGC_CodeEditorCore::OnScriptHorizontalScroll)
	.Orientation(Orient_Horizontal).AlwaysShowScrollbar(true)
	.Thickness(FVector2D(8.f,8.f));
	//
	AUTOCOMPLETE_SCROLL = SNew(SScrollBar)
	.OnUserScrolled(this,&SMGC_CodeEditorCore::OnInternalAutoCompleteScroll)
	.Thickness(FVector2D(8.f,8.f)).AlwaysShowScrollbar(false)
	.Orientation(Orient_Vertical);
	//
	//
	TSharedPtr<SOverlay>OverlayWidget;
	//
	ChildSlot[
	SNew(SHorizontalBox)
	+SHorizontalBox::Slot()
	.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
	[
		SAssignNew(OverlayWidget,SOverlay)
		+SOverlay::Slot()
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				SNew(SBox)
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				.MinDesiredWidth(500.f).MinDesiredHeight(300.f)
				[
					SNew(SBorder)
					.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
					.BorderImage(FEditorStyle::GetBrush("ToolPanel.DarkGroupBorder"))
					[
						SAssignNew(VS_SCROLL_BOX,SScrollBox)
						.OnUserScrolled(this,&SMGC_CodeEditorCore::OnVerticalScroll)
						.Orientation(EOrientation::Orient_Vertical)
						.ScrollBarThickness(FVector2D(8.f,8.f))
						+SScrollBox::Slot()
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.VAlign(VAlign_Fill).HAlign(HAlign_Left).AutoWidth()
							[
								SNew(SBorder)
								.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
								.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
								[
									SAssignNew(LINE_COUNTER,SListView<TSharedPtr<FString>>)
									.OnSelectionChanged(this,&SMGC_CodeEditorCore::OnSelectedLineCounterItem)
									.OnGenerateRow(this,&SMGC_CodeEditorCore::OnGenerateLineCounter)
									.ScrollbarVisibility(EVisibility::Collapsed)
									.ListItemsSource(&LineCount).ItemHeight(14)
									.SelectionMode(ESelectionMode::Single)
								]
							]
							+SHorizontalBox::Slot()
							.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
							[
								SAssignNew(SCRIPT_EDITOR,SKMGC_TextEditorWidget)
								.OnTextChanged(this,&SMGC_CodeEditorCore::OnScriptTextChanged,ETextCommit::Default)
								.OnTextCommitted(this,&SMGC_CodeEditorCore::OnScriptTextComitted)
								.OnInvokeSearch(this,&SMGC_CodeEditorCore::OnInvokedSearch)
								.OnAutoComplete(this,&SMGC_CodeEditorCore::OnAutoComplete)
								.IsEnabled(this,&SMGC_CodeEditorCore::HasScript)
								.Text(this,&SMGC_CodeEditorCore::GetScriptText)
								.VScrollBar(VS_SCROLL).HScrollBar(HS_SCROLL)
								.Marshaller(MARSHALL.ToSharedRef())
								.CanKeyboardFocus(true)
								.IsReadOnly(false)
							]
						]
					]
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight().Padding(0,5,0,0)
			[
				SNew(SBorder)
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
				[
					SNew(STextBlock).Margin(FMargin(5,0,0,0))
					.Text(this,&SMGC_CodeEditorCore::GetCursorLocation)
					.ColorAndOpacity(FSlateColor(FLinearColor(FColor(225,225,255,225))))
				]
			]
			+SVerticalBox::Slot()
			.AutoHeight().Padding(0,4,2,0)
			[
				SNew(SBorder)
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
				.Visibility(this,&SMGC_CodeEditorCore::GetDatabaseWarningVisibility)
				[
					SNew(SVerticalBox)
					+SVerticalBox::Slot()
					.AutoHeight().Padding(0)
					[
						SNew(STextBlock).Margin(FMargin(5,0,0,0))
						.Text(this,&SMGC_CodeEditorCore::GetDatabaseLoadTooltip)
						.ColorAndOpacity(FSlateColor(FLinearColor(1.f,0.45f,0.f)))
					]
					+SVerticalBox::Slot()
					.AutoHeight().Padding(2,0,2,0)
					[
						SNew(SProgressBar)
						.BorderPadding(FVector2D::ZeroVector)
						.Percent(this,&SMGC_CodeEditorCore::GetDatabaseLoad)
						.FillColorAndOpacity(FSlateColor(FLinearColor(0.5f,0.5f,1.f)))
					]
				]
			]
		]
		+SOverlay::Slot()
		.VAlign(VAlign_Top).HAlign(HAlign_Right)
		[
			SNew(SBox)
			.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
			.WidthOverride(250.f).HeightOverride(80.f)
			.Visibility(this,&SMGC_CodeEditorCore::GetSearchBoxVisibility)
			[
				SNew(SBorder)
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				.BorderImage(FEditorStyle::GetBrush("Sequencer.Thumbnail.SectionHandle"))
				[
					SNew(SBorder)
					.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
					.BorderImage(FEditorStyle::GetBrush("Menu.Background"))
					[
						SNew(SVerticalBox)
						+SVerticalBox::Slot().Padding(5)
						.VAlign(VAlign_Top).HAlign(HAlign_Fill)
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.Padding(0,2,5,0).AutoWidth()
							.VAlign(VAlign_Top).HAlign(HAlign_Left)
							[
								SNew(SCheckBox)
								.OnCheckStateChanged(this,&SMGC_CodeEditorCore::OnSearchSensitiveChanged)
								.IsChecked(this,&SMGC_CodeEditorCore::IsSearchSensitive)
								.Style(FEditorStyle::Get(),"ToggleButtonCheckbox")
								.Content()
								[
									SNew(STextBlock).Margin(2)
									.Text(LOCTEXT("KMGC_SearchToggleCase","Aa"))
									.ColorAndOpacity(FSlateColor(FLinearColor(FColor(255,255,255,225))))
									.Font(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font)
								]
							]
							+SHorizontalBox::Slot()
							.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
							[
								SAssignNew(SEARCH_TEXT,SEditableTextBox)
								.OnTextChanged(this,&SMGC_CodeEditorCore::OnSearchTextChanged,ETextCommit::Default)
								.OnTextCommitted(this,&SMGC_CodeEditorCore::OnSearchTextComitted)
								.Text(this,&SMGC_CodeEditorCore::GetSearchText)
								.SelectAllTextWhenFocused(true)
							]
							+SHorizontalBox::Slot()
							.Padding(5,2,0,0).AutoWidth()
							.VAlign(VAlign_Top).HAlign(HAlign_Left)
							[
								SNew(SButton)
								.OnClicked(this,&SMGC_CodeEditorCore::OnClickedSearchGlass)
								.ButtonStyle(FEditorStyle::Get(),"NoBorder")
								[
									SNew(SImage)
									.Image(FEditorStyle::Get().GetBrush(TEXT("Symbols.SearchGlass")))
								]
							]
						]
						+SVerticalBox::Slot().Padding(5)
						.VAlign(VAlign_Top).HAlign(HAlign_Fill)
						[
							SNew(SHorizontalBox)
							+SHorizontalBox::Slot()
							.Padding(5,0,5,0).AutoWidth()
							.VAlign(VAlign_Top).HAlign(HAlign_Left)
							[
								SNew(SBox)
								.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
								.WidthOverride(22.f).HeightOverride(22.f)
							]
							+SHorizontalBox::Slot()
							.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
							[
								SNew(SEditableTextBox)
								.SelectAllTextWhenFocused(true)
								.Text(this,&SMGC_CodeEditorCore::GetReplaceText)
								.OnTextCommitted(this,&SMGC_CodeEditorCore::OnReplaceTextComitted)
							]
							+SHorizontalBox::Slot()
							.Padding(5,0,0,5).AutoWidth()
							.VAlign(VAlign_Top).HAlign(HAlign_Left)
							[
								SNew(SButton)
								.OnClicked(this,&SMGC_CodeEditorCore::OnClickedReplaceSearch)
								.ButtonStyle(FEditorStyle::Get(),"NoBorder")
								[
									SNew(STextBlock).Margin(2)
									.Text(FText::FromString(FString(TEXT("\xf061"))))
									.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.11"))
									.ColorAndOpacity(FSlateColor(FLinearColor(FColor(255,255,255,225))))
								]
							]
						]
					]
				]
			]
		]
	]
	+SHorizontalBox::Slot()
	.HAlign(HAlign_Right).AutoWidth().Padding(FMargin(2.f,0.f,0.f,0.f))
	[
		SNew(SBorder)
		.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
		.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
		[
			SNew(SBox)
			.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
			.Visibility(this,&SMGC_CodeEditorCore::GetAutoCompleteVisibility)
			.MinDesiredWidth(50.f).MinDesiredHeight(250.f).MaxDesiredWidth(250.f)
			[
				SAssignNew(AUTOCOMPLETE_BOX,SScrollBox)
				.OnUserScrolled(this,&SMGC_CodeEditorCore::OnAutoCompleteScroll)
				.Orientation(EOrientation::Orient_Vertical)
				.ScrollBarThickness(FVector2D(8.f,8.f))
				//
				+SScrollBox::Slot()
				.VAlign(VAlign_Fill).HAlign(HAlign_Fill)
				[
					SAssignNew(AUTOCOMPLETE,SListView<TSharedPtr<FString>>)
					.OnMouseButtonDoubleClick(this,&SMGC_CodeEditorCore::OnClickedAutoCompleteItem)
					.OnSelectionChanged(this,&SMGC_CodeEditorCore::OnSelectedAutoCompleteItem)
					.OnGenerateRow(this,&SMGC_CodeEditorCore::OnGenerateAutoComplete)
					.ExternalScrollbar(AUTOCOMPLETE_SCROLL)
					.SelectionMode(ESelectionMode::Single)
					.ListItemsSource(&AutoCompleteList)
				]
			]
		]
	]
	];
	//
	//
	UpdateTextEditorScriptReference();
	//
	if (IsSourceView()) {
		SCRIPT_EDITOR->SetIsReadOnly(true);
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_CodeEditorCore::Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) {
	//
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SMGC_CodeEditorCore::HasScript() const {
	if (IsSourceView()) {return true;}
	//
	return (ScriptObject!=nullptr);
}

bool SMGC_CodeEditorCore::IsSourceView() const {
	return (!ExternalFilePath->IsEmpty());
}

bool SMGC_CodeEditorCore::IsScriptEditable() const {
	if (IsSourceView()) {return false;}
	//
	return !(ScriptObject->LockSourceCode);
}

int32 SMGC_CodeEditorCore::GetLineCount() const {
	if (IsSourceView()) {return 0;}
	//
	TArray<FString>Array;
	int32 Count = 0;
	//
	const FString Text = GetScriptText().ToString();
	Count = Text.ParseIntoArray(Array,TEXT("\n"),false);
	//
	return Count;
}

FText SMGC_CodeEditorCore::GetScriptText() const {
	if (IsSourceView()) {return FText::FromString(*ExternalFileText.Get());}
	//
	if (Source==EMGC_CodeSource::Script) {
		return FText::FromString(ScriptObject->Source.Script);
	} else if (Source==EMGC_CodeSource::Header) {
		return FText::FromString(ScriptObject->Source.Header);
	}///
	//
	return FText::FromString(ScriptObject->Source.Types);
}

FText SMGC_CodeEditorCore::GetSearchText() const {
	if (!SCRIPT_EDITOR.IsValid()) {return FText::FromString(TEXT("Search.."));}
	//
	return SCRIPT_EDITOR->GetSearchText();
}

FText SMGC_CodeEditorCore::GetReplaceText() const {
	return FText::FromString(ReplaceText);
}

FText SMGC_CodeEditorCore::GetCursorLocation() const {
	if (!SCRIPT_EDITOR.IsValid()) {return FText();}
	//
	return FText::FromString(FString::Printf(TEXT("Ln: %i  |  Col: %i"),SCRIPT_EDITOR->GetCursorLocation().GetLineIndex()+1,SCRIPT_EDITOR->GetCursorLocation().GetOffset()+1));
}

void SMGC_CodeEditorCore::SetLineCountList(const int32 Count) {
	LineCount.Empty();
	//
	for (int32 I=1; I<=Count; I++) {
		FString ID = FString::Printf(TEXT("%i"),I);
		LineCount.Add(MakeShareable(new FString(*ID)));
	}///
	//
	if (LINE_COUNTER.IsValid()) {LINE_COUNTER->RequestListRefresh();}
}

void SMGC_CodeEditorCore::SetScriptText(const FText &NewText) {
	if (!IsScriptEditable()) {return;}
	//
	if (Source==EMGC_CodeSource::Script) {
		if (ScriptObject->Source.Script != NewText.ToString()) {
			ScriptObject->Modify();
			ScriptObject->Source.Script = NewText.ToString();
		}///
	} else if (Source==EMGC_CodeSource::Header) {
		if (ScriptObject->Source.Header != NewText.ToString()) {
			ScriptObject->Modify();
			ScriptObject->Source.Header = NewText.ToString();
		}///
	} else {
		if (ScriptObject->Source.Types != NewText.ToString()) {
			ScriptObject->Modify();
			ScriptObject->Source.Types = NewText.ToString();
		}///
	}///
}

void SMGC_CodeEditorCore::SetAutoCompleteList(const TArray<FString>&List) {
	AutoCompleteList.Empty();
	//
	for (const FString &S : List) {
		if (S.TrimStartAndEnd().IsEmpty()) {continue;}
		AutoCompleteList.Add(MakeShareable(new FString(S)));
	}///
	//
	if (AUTOCOMPLETE.IsValid()) {AUTOCOMPLETE->RequestListRefresh();}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SMGC_CodeEditorCore::CanBuildDatabase() const {
	return (UMGC_SemanticDB::DBState==EDatabaseState::READY);
}

FText SMGC_CodeEditorCore::GetDatabaseLoadTooltip() const {
	if (DatabaseLoad<=0) {return FText::FromString(TEXT("Loading Unreal Types "));}
	if (DatabaseLoad>=0.80f) {return FText::FromString(TEXT("Loading Unreal Types .. .. .. .."));}
	if ((DatabaseLoad>=0.20f)&&(DatabaseLoad<0.40f)) {return FText::FromString(TEXT("Loading Unreal Types .."));}
	if ((DatabaseLoad>=0.40f)&&(DatabaseLoad<0.60f)) {return FText::FromString(TEXT("Loading Unreal Types .. .."));}
	if ((DatabaseLoad>=0.60f)&&(DatabaseLoad<0.80f)) {return FText::FromString(TEXT("Loading Unreal Types .. ... .."));}
	//
	return FText::FromString(TEXT("Loading Unreal Types ...."));
}

TOptional<float>SMGC_CodeEditorCore::GetDatabaseLoad() const {
	return TOptional<float>(DatabaseLoad);
}

ECheckBoxState SMGC_CodeEditorCore::IsSearchSensitive() const {
	return (SensitiveSearch) ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
}

EVisibility SMGC_CodeEditorCore::GetSearchBoxVisibility() const {
	if (!HasScript()) {return EVisibility::Collapsed;}
	//
	return (ViewSearchBox) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SMGC_CodeEditorCore::GetDatabaseWarningVisibility() const {
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {
		return EVisibility::Visible;
	}///
	//
	return EVisibility::Collapsed;
}

EVisibility SMGC_CodeEditorCore::GetAutoCompleteVisibility() const {
	if (!SCRIPT_EDITOR.IsValid()) {return EVisibility::Collapsed;}
	if (!HasScript()) {return EVisibility::Collapsed;}
	//
	if (AutoComplete==EAutoComplete::Active) {
		return EVisibility::Visible;
	}///
	//
	return EVisibility::Collapsed;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FReply SMGC_CodeEditorCore::OnClickedSearchGlass() {
	if (!HasScript()) {return FReply::Unhandled();}
	//
	const ESearchCase::Type Case = (SensitiveSearch) ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;
	if (ReplaceText==TEXT("Replace..")) {ReplaceText=TEXT("");}
	//
	FSlateApplication::Get().SetKeyboardFocus(SCRIPT_EDITOR.ToSharedRef());
	SCRIPT_EDITOR->BeginSearch(FText::FromString(SearchText),Case,false);
	//
	return FReply::Handled();
}

FReply SMGC_CodeEditorCore::OnClickedReplaceSearch() {
	if (!SCRIPT_EDITOR.IsValid()) {return FReply::Unhandled();}
	if (ReplaceText.IsEmpty()) {return FReply::Handled();}
	if (SearchText.IsEmpty()) {return FReply::Handled();}
	if (!HasScript()) {return FReply::Unhandled();}
	//
	//
	FText DO;
	if (Source==EMGC_CodeSource::Header) {
		DO = FText::FromString(ScriptObject->Source.Header.Replace(*SearchText,*ReplaceText));
	} else if (Source==EMGC_CodeSource::Script) {
		DO = FText::FromString(ScriptObject->Source.Script.Replace(*SearchText,*ReplaceText));
	} else if (Source==EMGC_CodeSource::Types) {
		DO = FText::FromString(ScriptObject->Source.Types.Replace(*SearchText,*ReplaceText));
	}///
	//
	SCRIPT_EDITOR->BeginEditTransaction();
	 SCRIPT_EDITOR->SetText(DO);
	SCRIPT_EDITOR->EndEditTransaction();
	//
	return FReply::Handled();
}

void SMGC_CodeEditorCore::OnClickedAutoCompleteItem(TSharedPtr<FString>Item) {
	if (!IsScriptEditable()) {return;}
	//
	if (AUTOCOMPLETE.IsValid()&&SCRIPT_EDITOR.IsValid()) {
		FSlateApplication::Get().SetKeyboardFocus(SCRIPT_EDITOR.ToSharedRef());
		//
		if (Item.IsValid()) {
			FString Result;
			Item.Get()->Split(TEXT("|"),nullptr,&Result,ESearchCase::IgnoreCase,ESearchDir::FromEnd);
			//
			Result.ReplaceInline(*SCRIPT_EDITOR->GetUnderCursor(),TEXT(""));
			//
			if (Item.Get()->Contains(TEXT("FUN|"))) {
				SCRIPT_EDITOR->InsertTextAtCursor(Result+TEXT("()"));
			} else {SCRIPT_EDITOR->InsertTextAtCursor(Result);}
		}///
	}///
	//
	if (Item.IsValid()) {AUTOCOMPLETE->SetItemSelection(Item,false);}
}

void SMGC_CodeEditorCore::OnSelectedAutoCompleteItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo) {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_CodeEditorCore::OnScriptHorizontalScroll(float Offset) {}
void SMGC_CodeEditorCore::OnInternalVerticalScroll(float Offset) {
	VS_SCROLL_BOX->SetScrollOffset(Offset);
}

void SMGC_CodeEditorCore::OnVerticalScroll(float Offset) {
	VS_SCROLL->SetState(VS_SCROLL_BOX->GetScrollOffset(),VS_SCROLL_BOX->GetViewOffsetFraction());
}

void SMGC_CodeEditorCore::OnAutoCompleteScroll(float Offset) {
	AUTOCOMPLETE_SCROLL->SetState(AUTOCOMPLETE_BOX->GetScrollOffset(),AUTOCOMPLETE_BOX->GetViewOffsetFraction());
}

void SMGC_CodeEditorCore::OnInternalAutoCompleteScroll(float Offset) {
	AUTOCOMPLETE_BOX->SetScrollOffset(Offset);
}

void SMGC_CodeEditorCore::OnScriptTextChanged(const FText &InText, ETextCommit::Type CommitType) {
	if (!IsScriptEditable()) {return;}
	//
	TArray<FString>Lines;
	InText.ToString().ParseIntoArrayLines(Lines,false);
	//
	if (SCRIPT_EDITOR->HasAutoComplete()) {
		FString Subject = SCRIPT_EDITOR->ParseAutoCompleteWord(Lines,true);
		if (AutoCompleteList.Num()>=1) {OnAdvanceAutoComplete(Subject);}
		else {SCRIPT_EDITOR->AutoCompleteSubject(Subject);}
	} else {
		FString Subject = SCRIPT_EDITOR->ParseAutoCompleteWord(Lines);
		if (SCRIPT_EDITOR->IsAutoComplete(Subject)) {
			SCRIPT_EDITOR->AutoCompleteSubject(Subject);
		} else {SCRIPT_EDITOR->AutoSuggest(Lines);}
	}///
	//
	SetScriptText(InText);
	SetLineCountList(GetLineCount());
}

void SMGC_CodeEditorCore::OnScriptTextComitted(const FText &NewText, ETextCommit::Type CommitInfo) {
	if (!IsScriptEditable()) {return;}
	//
	SetScriptText(NewText);
	SetLineCountList(GetLineCount());
}

void SMGC_CodeEditorCore::OnReplaceTextComitted(const FText &NewText, ETextCommit::Type CommitInfo) {
	ReplaceText.Empty(); ReplaceText.Append(NewText.ToString());
}

void SMGC_CodeEditorCore::OnSelectedLineCounterItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo) {
	if (!Item.IsValid()) {return;}
	//
	const int32 LineID = FCString::Atoi(**Item.Get());
	LINE_COUNTER->SetItemSelection(Item,false);
	//
	FSlateApplication::Get().SetKeyboardFocus(SCRIPT_EDITOR.ToSharedRef());
	SCRIPT_EDITOR->GoToLineColumn(LineID-1,0);
	SCRIPT_EDITOR->SelectLine();
}

TSharedRef<ITableRow> SMGC_CodeEditorCore::OnGenerateLineCounter(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable) {
	return
	SNew(SComboRow<TSharedRef<FString>>,OwnerTable)
	[
		SNew(SBorder)
		.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
		.ColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f))
		.Padding(FMargin(5.f,0.f,5.f,0.f))
		[
			SNew(STextBlock)
			.Text(FText::FromString(*Item.Get()))
			.ColorAndOpacity(FSlateColor(FLinearColor(FColor(75,185,245,225))))
			.Font(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font)
		]
	];//
}

TSharedRef<ITableRow> SMGC_CodeEditorCore::OnGenerateAutoComplete(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable) {
	if (Item.IsValid()&&(Item.Get()->Contains(TEXT("FUN|")))) {
		FString Caption;
		//
		Item.Get()->Split(TEXT("|"),nullptr,&Caption,ESearchCase::IgnoreCase,ESearchDir::FromEnd);
		//
		uint8 R = 155; uint8 G = 155; uint8 B = 155;
		if (Item.Get()->Contains(TEXT("PB|"))) {R=255, G=125, B=5;}
		if (Item.Get()->Contains(TEXT("PT|"))) {R=55, G=175, B=255;}
		if (Item.Get()->Contains(TEXT("PV|"))) {R=25, G=125, B=225;}
		if (Item.Get()->Contains(TEXT("ST|"))) {R=125, G=125, B=125;}
		FSlateColor Color = FSlateColor(FLinearColor(FColor(R,G,B,255)));
		//
		return SNew(SComboRow<TSharedRef<FString>>,OwnerTable)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth().VAlign(VAlign_Center).Padding(4,0,4,0)
			[
				SNew(SImage)
				.ColorAndOpacity(Color)
				.Image(FEditorStyle::GetBrush(TEXT("Kismet.AllClasses.FunctionIcon")))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SBorder)
				.ColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f))
				.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
				[
					SNew(STextBlock)
					.ColorAndOpacity(Color)
					.Margin(FMargin(2.f,2.f,2.f,2.f))
					.Text(FText::FromString(Caption))
					.Font(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font)
				]
			]
		];//
	} else if (Item.IsValid()&&(Item.Get()->Contains(TEXT("VAR|")))) {
		FString Caption;
		//
		Item.Get()->Split(TEXT("|"),nullptr,&Caption,ESearchCase::IgnoreCase,ESearchDir::FromEnd);
		//
		return SNew(SComboRow<TSharedRef<FString>>,OwnerTable)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth().VAlign(VAlign_Center).Padding(4,0,4,0)
			[
				SNew(SImage)
				.Image(IKMGC_ScriptParser::GetTypeIcon(*Item.Get()))
				.ColorAndOpacity(IKMGC_ScriptParser::GetTypeColor(*Item.Get()))
			]
			+SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SBorder)
				.ColorAndOpacity(FLinearColor(1.f,1.f,1.f,1.f))
				.BorderImage(FEditorStyle::GetBrush("Graph.Node.Body"))
				[
					SNew(STextBlock)
					.Margin(FMargin(2.f,2.f,2.f,2.f))
					.Text(FText::FromString(Caption))
					.ColorAndOpacity(FSlateColor(FLinearColor(FColor(255,235,195,255))))
					.Font(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font)
				]
			]
		];//
	}///
	//
	return SNew(SComboRow<TSharedRef<FString>>,OwnerTable);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_CodeEditorCore::OnAutoComplete(const TArray<FString>&Results) {
	if (!IsScriptEditable()) {return;}
	//
	if (Results.Num()<=0) {
		SetAutoCompleteList(Results);
		AutoComplete=EAutoComplete::Off;
	return;}
	//
	TArray<FString>Results_PROP;
	TArray<FString>Results_FUN;
	//
	for (const FString &Item : Results) {
		if (Item.Contains(TEXT("VAR|"))) {Results_PROP.Add(Item);} else
		if (Item.Contains(TEXT("FUN|"))) {Results_FUN.Add(Item);}
	}///
	//
	Results_PROP.Append(Results_FUN);
	SetAutoCompleteList(Results_PROP);
	//
	if (Results_PROP.Num()>=1||Results_FUN.Num()>=1) {AutoComplete=EAutoComplete::Active;} else {AutoComplete=EAutoComplete::Off;}
}

void SMGC_CodeEditorCore::OnAdvanceAutoComplete(const FString &Search) {
	if (!IsScriptEditable()) {return;}
	//
	if (AutoComplete==EAutoComplete::Off) {return;}
	if (AutoCompleteList.Num()==0) {return;}
	//
	TArray<FString>Found;
	for (const TSharedPtr<FString>&Item : AutoCompleteList) {
		if (!Item.IsValid()) {continue;}
		//
		if (Item->Contains(Search)) {Found.Add(**Item.Get());}
	}///
	//
	if (Found.Num()>=1) {SetAutoCompleteList(Found);}
}

void SMGC_CodeEditorCore::OnInvokedSearch(bool DoSearch) {
	ReplaceText = TEXT("Replace..");
	ViewSearchBox = DoSearch;
	//
	if (ViewSearchBox) {
		FSlateApplication::Get().SetKeyboardFocus(SEARCH_TEXT.ToSharedRef());
	}///
}

void SMGC_CodeEditorCore::OnSearchSensitiveChanged(ECheckBoxState NewState) {
	SensitiveSearch = (NewState==ECheckBoxState::Checked);
}

void SMGC_CodeEditorCore::OnSearchTextChanged(const FText &InText, ETextCommit::Type CommitType) {
	SearchText = InText.ToString();
}

void SMGC_CodeEditorCore::OnSearchTextComitted(const FText &NewText, ETextCommit::Type CommitInfo) {
	SearchText.Empty(); SearchText.Append(NewText.ToString());
	//
	const ESearchCase::Type Case = (SensitiveSearch) ? ESearchCase::CaseSensitive : ESearchCase::IgnoreCase;
	SCRIPT_EDITOR->BeginSearch(FText::FromString(SearchText),Case,false);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_CodeEditorCore::GoToTextLocation(const FTextLocation &Location) {
	if (SCRIPT_EDITOR.IsValid()) {
		FSlateApplication::Get().SetKeyboardFocus(SCRIPT_EDITOR.ToSharedRef());
		//
		if (Location.IsValid()) {
			SCRIPT_EDITOR->GoToLineColumn(Location.GetLineIndex(),Location.GetOffset());
			SCRIPT_EDITOR->SelectLine();
		}///
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_CodeEditorCore::UpdateDatabaseReferences() {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	TArray<UMGC_SemanticDB*>SemanticDB;
	TArray<UMGC_FunctionDB*>FunctionDB;
	TArray<UMGC_KeywordDB*>KeywordDB;
	TArray<UMGC_ClassDB*>ClassDB;
	//
	for (auto DB : _Settings->KeywordDB.Array()) {
		if (DB.IsValid()) {KeywordDB.Add(DB.LoadSynchronous());}
	}///
	//
	for (auto DB : _Settings->ClassDB.Array()) {
		if (DB.IsValid()) {ClassDB.Add(DB.LoadSynchronous());}
	}///
	//
	for (auto DB : _Settings->FunctionDB.Array()) {
		if (DB.IsValid()) {FunctionDB.Add(DB.LoadSynchronous());}
	}///
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {SemanticDB.Add(DB.LoadSynchronous());}
	}///
	//
	//
	MARSHALL = FKMGC_TextSyntaxHighlighter::Create(
		FKMGC_TextSyntaxHighlighter::FSyntaxTextStyle(KeywordDB,ClassDB,FunctionDB,SemanticDB)
	);///
	//
	SetLineCountList(GetLineCount());
}

void SMGC_CodeEditorCore::UpdateDatabaseSemantics() {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {return;}
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			(new FAutoDeleteAsyncTask<TASK_BuildAutoCompleteData>(DB.LoadSynchronous()))->StartBackgroundTask();
		}///
	}///
}

void SMGC_CodeEditorCore::UpdateTextEditorScriptReference() {
	if (IsSourceView()) {return;} if (!HasScript()) {return;}
	//
	if (SCRIPT_EDITOR.IsValid()) {
		SCRIPT_EDITOR->SetScriptObject(ScriptObject);
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////