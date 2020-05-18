//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "KMGC_TextEditorWidget.h"

#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Async/AsyncWork.h"
#include "Runtime/Core/Public/Async/TaskGraphInterfaces.h"

#include "Runtime/Slate/Public/Widgets/Layout/SBox.h"
#include "Runtime/SlateCore/Public/Widgets/SOverlay.h"
#include "Runtime/Slate/Public/Widgets/Input/SButton.h"
#include "Runtime/Slate/Public/Widgets/Layout/SBorder.h"
#include "Runtime/Slate/Public/Widgets/Views/SListView.h"
#include "Runtime/Slate/Public/Widgets/Input/SCheckBox.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBar.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBox.h"
#include "Runtime/SlateCore/Public/Widgets/Images/SImage.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableText.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableTextBox.h"

#include "Editor/EditorStyle/Public/EditorStyle.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMagicNodeScript;
class FKMGC_NodeStyle;
class UMagicNode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TASK_LoadFile : public FNonAbandonableTask {
public:
	TSharedPtr<FString>TargetFilePath;
	TSharedPtr<FString>TargetString;
public:
	TASK_LoadFile(const TSharedPtr<FString>&Output, const TSharedPtr<FString>&Path) {
		TargetFilePath = Path; TargetString = Output;
	}///
	//
	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(TASK_LoadFile,STATGROUP_ThreadPoolAsyncTasks);
	}///
	//
	void DoWork() {
		FFileHelper::LoadFileToString(TargetString.ToSharedRef().Get(),**TargetFilePath.Get());
	}///
	//
	bool CanAbandon() {return true;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SMGC_CodeEditorCore : public SCompoundWidget {
private:
	TSharedPtr<SEditableTextBox>SEARCH_TEXT;
	TSharedPtr<ITextLayoutMarshaller>MARSHALL;
	TSharedPtr<SKMGC_TextEditorWidget>SCRIPT_EDITOR;
	TSharedPtr<SListView<TSharedPtr<FString>>>LINE_COUNTER;
	TSharedPtr<SListView<TSharedPtr<FString>>>AUTOCOMPLETE;
	//
	TSharedPtr<SScrollBar>AUTOCOMPLETE_SCROLL;
	TSharedPtr<SScrollBox>AUTOCOMPLETE_BOX;
	//
	TSharedPtr<SScrollBox>VS_SCROLL_BOX;
	TSharedPtr<SScrollBar>HS_SCROLL;
	TSharedPtr<SScrollBar>VS_SCROLL;
	//
	UMagicNodeScript* ScriptObject;
private:
	EMGC_CodeSource Source;
	EAutoComplete AutoComplete;
	//
	TArray<TSharedPtr<FString>>LineCount;
	TArray<TSharedPtr<FString>>AutoCompleteList;
	//
	TSharedPtr<FString>ExternalFilePath;
	TSharedPtr<FString>ExternalFileText;
	//
	FString SearchText;
	FString ReplaceText;
	//
	float DatabaseLoad;
	bool ViewSearchBox;
	bool SensitiveSearch;
	bool RequestedUpdateDB;
public:
	SMGC_CodeEditorCore();
	~SMGC_CodeEditorCore();
	//
	SLATE_BEGIN_ARGS(SMGC_CodeEditorCore)
	{}///
		SLATE_ARGUMENT(EMGC_CodeSource,SourceToEdit)
		SLATE_ARGUMENT(FString,ExternalSourcePath)
	SLATE_END_ARGS()
	//
	//
	void Construct(const FArguments &InArgs, UMagicNodeScript* InScriptObject);
	void Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) override;
	//
	//
	bool HasScript() const;
	bool IsSourceView() const;
	bool IsScriptEditable() const;
	bool CanBuildDatabase() const;
	//
	int32 GetLineCount() const;
	FText GetScriptText() const;
	FText GetSearchText() const;
	FText GetReplaceText() const;
	FText GetCursorLocation() const;
	//
	void SetLineCountList(const int32 Count);
	void SetScriptText(const FText &NewText);
	void SetAutoCompleteList(const TArray<FString>&List);
	void OnScriptTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnScriptTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnReplaceTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnSelectedLineCounterItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo);
	//
	void OnInvokedSearch(bool DoSearch);
	void OnAdvanceAutoComplete(const FString &Search);
	void OnAutoComplete(const TArray<FString>&Results);
	void OnSearchSensitiveChanged(ECheckBoxState NewState);
	void OnSearchTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnSearchTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	//
	void OnVerticalScroll(float Offset);
	void OnAutoCompleteScroll(float Offset);
	void OnInternalVerticalScroll(float Offset);
	void OnScriptHorizontalScroll(float Offset);
	void OnInternalAutoCompleteScroll(float Offset);
	//
	TSharedRef<ITableRow>OnGenerateLineCounter(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	TSharedRef<ITableRow>OnGenerateAutoComplete(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	//
	//
	FText GetDatabaseLoadTooltip() const;
	TOptional<float>GetDatabaseLoad() const;
	ECheckBoxState IsSearchSensitive() const;
	//
	EVisibility GetSearchBoxVisibility() const;
	EVisibility GetAutoCompleteVisibility() const;
	EVisibility GetDatabaseWarningVisibility() const;
	//
	FReply OnClickedSearchGlass();
	FReply OnClickedReplaceSearch();
	//
	void OnClickedAutoCompleteItem(TSharedPtr<FString>Item);
	void OnSelectedAutoCompleteItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo);
	//
	void GoToTextLocation(const FTextLocation &Location);
	//
	//
	void UpdateDatabaseSemantics();
	void UpdateDatabaseReferences();
	void UpdateTextEditorScriptReference();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////