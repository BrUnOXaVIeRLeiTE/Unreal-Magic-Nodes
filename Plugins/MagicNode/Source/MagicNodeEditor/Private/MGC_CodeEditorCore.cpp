//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MGC_CodeEditorCore.h"

#include "KMGC_NodeStyle.h"
#include "KMGC_NodeWidget.h"
#include "KMGC_TextSyntaxHighlighter.h"

#include "Runtime/Core/Public/Misc/FileHelper.h"
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
	///NavigationBar = InArgs._GraphNavigationBar;
	///ToolBox = InArgs._GraphToolBox;
	//
	//
	Source = InArgs._SourceToEdit;
	ExternalFilePath = MakeShareable(new FString(InArgs._ExternalSourcePath));
	//
	if (IsSourceView()) {
		FFileHelper::LoadFileToString(ExternalFileText.ToSharedRef().Get(),**ExternalFilePath.Get());
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
	//
	/*if (!NavigationBar.IsValid()) {
	SAssignNew(NavigationBar,SBorder)
	.Visibility(this,&SMGC_CodeEditorCore::HiddenIfGraphReadOnly)
	.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
	.HAlign(HAlign_Fill)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.Padding(0.0f,0.0f,3.0f,0.0f)
		.AutoWidth()
		[
			//SNew(SErrorText)
			//.ErrorText(FText::FromString(TEXT("derp DERRRRRP!!!!!!!!!!!")))
			//.Visibility(ViewModel.ToSharedRef(),&FNiagaraScriptGraphViewModel::GetGraphErrorTextVisible)
			//.BackgroundColor(ViewModel.ToSharedRef(),&FNiagaraScriptGraphViewModel::GetGraphErrorColor)
			//.ToolTipText(ViewModel.ToSharedRef(),&FNiagaraScriptGraphViewModel::GetGraphErrorMsgToolTip)
			//.ErrorText(ViewModel->GetGraphErrorText())
		]
		+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		[
			//SNew(STextBlock).Margin(8.f)
			//.Text(FText::FromString(TEXT("EeeeeeeeeeeeeeEEERPP !!!!")))
			//.Text(ViewModel.ToSharedRef(),&FNiagaraScriptGraphViewModel::GetDisplayName)
			//.TextStyle(FEditorStyle::Get(),TEXT("GraphBreadcrumbButtonText"))
			//.Justification(ETextJustify::Center)
		]
	];}*/
	//
	//
	/*if (!ToolBox.IsValid()) {
		SAssignNew(ToolBox,SBorder)
		.Visibility(this,&SMGC_CodeEditorCore::HiddenIfGraphReadOnly)
		.BorderImage(FEditorStyle::GetBrush(TEXT("Graph.TitleBackground")))
		.HAlign(HAlign_Fill)
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.Padding(0.0f,0.0f,3.0f,0.0f)
			.AutoWidth()
			[
				SNew(SErrorText)
				.ErrorText(FText::FromString(TEXT("derp BOX!!!!!!!!!!!")))
			]
			+ SHorizontalBox::Slot()
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.ButtonStyle(FEditorStyle::Get(),"FlatButton.Default")
				//.OnClicked(this,&SQLPreviewButtonWidget::OnButtonClicked)
				.ForegroundColor(FSlateColor::UseForeground())
				.HAlign(HAlign_Fill).VAlign(VAlign_Center)
				[
					SNew(STextBlock).Margin(FMargin(10.f))
					.Font(FEditorStyle::GetFontStyle(TEXT("BoldFont")))
					.Text(FText::FromString(TEXT("Click ME !!!")))
					.Justification(ETextJustify::Center)
					//.ToolTipText(Tooltip)
				]
			]
		];
	}*///
	//
	//
	TSharedPtr<SOverlay>OverlayWidget;
	//
	this->ChildSlot
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
								//.OnInvokeSearch(this,&SMGC_CodeEditorCore::OnInvokedSearch)
								//.OnAutoComplete(this,&SMGC_CodeEditorCore::OnAutoComplete)
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
		//
		/*/ Title bar
		+SOverlay::Slot()
		.VAlign(VAlign_Top)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			[
				NavigationBar.IsValid() ? NavigationBar.ToSharedRef() : (TSharedRef<SWidget>)SNullWidget::NullWidget
			]
		]
		// Top-right corner text indicating PIE is active
		+SOverlay::Slot().Padding(20)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Top)
		[
			SNew(STextBlock)
			.Visibility(this,&SMGC_CodeEditorCore::VisibleIfGraphReadOnly)
			.TextStyle(FEditorStyle::Get(),"Graph.SimulatingText")
			.Text(FText::FromString(TEXT("|PIIIE EeEEERPP !!!!")))
			//.Text( PIENotifyText )
		]
		// ToolBox bar 
		+SOverlay::Slot()
		.VAlign(VAlign_Bottom)
		.HAlign(HAlign_Fill)
		[
			ToolBox.IsValid() ? ToolBox.ToSharedRef() : (TSharedRef<SWidget>)SNullWidget::NullWidget
		]
		// Bottom-left corner text for notification list position
		+SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Bottom)
		.Padding(15.f,15.f,15.f,45.f)
		[
			SAssignNew(NotificationList,SNotificationList)
			.Visibility(EVisibility::SelfHitTestInvisible)
		]*/
	];//
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
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {
		RequestedUpdateDB = true;
		//
		if (DatabaseLoad>=1.f) {
			DatabaseLoad = 0.f;
		} else {DatabaseLoad+=0.05f;}
	} else if (RequestedUpdateDB) {
		UpdateDatabaseReferences();
		RequestedUpdateDB = false;
		DatabaseLoad = 0.f;
		//
		Invalidate(EInvalidateWidgetReason::Layout);
	}///
}

/*TSharedPtr<SWidget> SMGC_CodeEditorCore::GetNavigationBar() const {
	return NavigationBar;
}

TSharedPtr<SWidget> SMGC_CodeEditorCore::GetToolBox() const {
	return ToolBox;
}

void SMGC_CodeEditorCore::AddNotification(FNotificationInfo &Info, bool Success) {
	Info.bUseLargeFont = true;
	//
	TSharedPtr<SNotificationItem>Notification = NotificationList->AddNotification(Info);
	if (Notification.IsValid()) {
		Notification->SetCompletionState(Success ? SNotificationItem::CS_Success : SNotificationItem::CS_Fail);
	}///
}*/

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

TOptional<float> SMGC_CodeEditorCore::GetDatabaseLoad() const {
	return TOptional<float>(DatabaseLoad);
}

EVisibility SMGC_CodeEditorCore::GetDatabaseWarningVisibility() const {
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {
		return EVisibility::Visible;
	}///
	//
	return EVisibility::Collapsed;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_CodeEditorCore::OnScriptHorizontalScroll(float Offset) {}
void SMGC_CodeEditorCore::OnInternalVerticalScroll(float Offset) {
	VS_SCROLL_BOX->SetScrollOffset(Offset);
}

void SMGC_CodeEditorCore::OnVerticalScroll(float Offset) {
	VS_SCROLL->SetState(VS_SCROLL_BOX->GetScrollOffset(),VS_SCROLL_BOX->GetViewOffsetFraction());
}

void SMGC_CodeEditorCore::OnScriptTextChanged(const FText &InText, ETextCommit::Type CommitType) {
	if (!IsScriptEditable()) {return;}
	//
	TArray<FString>Lines;
	InText.ToString().ParseIntoArrayLines(Lines,false);
	//
	FString Subject = SCRIPT_EDITOR->ParseAutoCompleteWord(Lines);
	SCRIPT_EDITOR->AutoSuggest(Lines);
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

void SMGC_CodeEditorCore::OnSelectedLineCounterItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo) {
	if (!Item.IsValid()) {return;}
	//
	const int32 LineID = FCString::Atoi(**Item.Get());
	//
	FSlateApplication::Get().SetKeyboardFocus(SCRIPT_EDITOR.ToSharedRef());
	SCRIPT_EDITOR->GoToLineColumn(LineID-1,0);
	SCRIPT_EDITOR->SelectLine();
	//
	LINE_COUNTER->SetItemSelection(Item,false);
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