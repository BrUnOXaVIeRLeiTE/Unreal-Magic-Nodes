//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeRuntime.h"
#include "KMGC_TextEditorWidget.h"

#include "Runtime/SlateCore/Public/Widgets/SOverlay.h"
#include "Runtime/SlateCore/Public/Widgets/Images/SImage.h"

#include "Runtime/Slate/Public/Widgets/Layout/SBox.h"
#include "Runtime/Slate/Public/Widgets/Input/SButton.h"
#include "Runtime/Slate/Public/Widgets/Layout/SBorder.h"
#include "Runtime/Slate/Public/Widgets/Views/SListView.h"
#include "Runtime/Slate/Public/Widgets/Input/SCheckBox.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBar.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBox.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableText.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableTextBox.h"
#include "Runtime/Slate/Public/Widgets/Notifications/SProgressBar.h"

#include "Editor/UnrealEd/Public/FileHelpers.h"
#include "Editor/GraphEditor/Public/SGraphNode.h"
#include "Editor/EditorStyle/Public/EditorStyle.h"
#include "Editor/Kismet/Public/WorkflowOrientedApp/SContentReference.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FKMGC_TextSyntaxHighlighter;
class UMagicNodeScript;
class UKMGC_MagicNode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class ESKMGC_Source : uint8 {
	Script,
	Header,
	Types
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SKMGC_MagicNodeWidget : public SGraphNode {
private:
	TSharedPtr<FKMGC_TextSyntaxHighlighter>MARSHALL;
	//
	TSharedPtr<SListView<TSharedPtr<FString>>>INCLUDES_LIST;
	TSharedPtr<SListView<TSharedPtr<FString>>>AUTOCOMPLETE;
	TSharedPtr<SListView<TSharedPtr<FString>>>LINE_COUNTER;
	TSharedPtr<SListView<TSharedPtr<FString>>>MACROS_LIST;
	TSharedPtr<SKMGC_TextEditorWidget>SCRIPT_EDITOR;
	TSharedPtr<SKMGC_TextEditorWidget>HEADER_EDITOR;
	TSharedPtr<SKMGC_TextEditorWidget>TYPES_EDITOR;
	TSharedPtr<SContentReference>SCRIPT_PICKER;
	TSharedPtr<SScrollBar>AUTOCOMPLETE_SCROLL;
	TSharedPtr<SEditableTextBox>SEARCH_TEXT;
	TSharedPtr<SScrollBox>AUTOCOMPLETE_BOX;
	TSharedPtr<SScrollBox>VS_SCROLL_BOX;
	TSharedPtr<SScrollBar>HSS_SCROLL;
	TSharedPtr<SScrollBar>HSH_SCROLL;
	TSharedPtr<SScrollBar>VS_SCROLL;
	//
	TArray<TSharedPtr<FString>>AutoCompleteList;
	TArray<TSharedPtr<FString>>SourceIncludes;
	TArray<TSharedPtr<FString>>SourceMacros;
	TArray<TSharedPtr<FString>>LineCount;
	//
	EAutoComplete AutoComplete;
	ESKMGC_Source Source;
	//
	FString ReplaceText;
	FString SearchText;
	FString LastHint;
	//
	bool ViewMacros;
	bool ViewIncludes;
	bool ViewSearchBox;
	bool SensitiveSearch;
	bool RequestedUpdateDB;
	//
	float HintTimer;
	float DatabaseLoad;
public:
	SKMGC_MagicNodeWidget();
	virtual ~SKMGC_MagicNodeWidget() override;
	//
	SLATE_BEGIN_ARGS(SKMGC_MagicNodeWidget){}
	SLATE_END_ARGS()
	//
	void Construct(const FArguments &InArgs, UKMGC_MagicNode* InGraphNode);
	//
	//
	bool HasScript() const;
	bool CanCompileScript() const;
	bool CanBuildDatabase() const;
	bool IsScriptSourceEditable() const;
	//
	int32 GetLineCount() const;
	FText GetTypesText() const;
	FText GetScriptText() const;
	FText GetHeaderText() const;
	FText GetSearchText() const;
	FText GetReplaceText() const;
	FText GetCursorLocation() const;
	FText GetDatabaseLoadTooltip() const;
	//
	UObject* GetScriptObject() const;
	//
	TArray<FString>GetScriptMacros() const;
	TArray<FString>GetScriptIncludes() const;
	//
	TOptional<float>GetDatabaseLoad() const;
	ECheckBoxState IsSearchSensitive() const;
	//
	//
	FReply OnClickedCompile();
	FReply OnClickedAddMacro();
	FReply OnClickedTypesIcon();
	FReply OnClickedSaveScript();
	FReply OnClickedScriptIcon();
	FReply OnClickedHeaderIcon();
	FReply OnClickedAddInclude();
	FReply OnClickedViewMacros();
	FReply OnClickedSearchGlass();
	FReply OnClickedReloadScript();
	FReply OnClickedViewIncludes();
	FReply OnClickedBuildDatabase();
	FReply OnClickedReplaceSearch();
	FReply OnClickedRemoveMacro(const FString Item);
	FReply OnClickedRemoveInclude(const FString Item);
	//
	EVisibility GetSearchBoxVisibility() const;
	EVisibility GetTypesPanelVisibility() const;
	EVisibility GetScriptPanelVisibility() const;
	EVisibility GetHeaderPanelVisibility() const;
	EVisibility GetMacrosPanelVisibility() const;
	EVisibility GetScriptEditorVisibility() const;
	EVisibility GetAutoCompleteVisibility() const;
	EVisibility GetIncludesPanelVisibility() const;
	EVisibility GetDatabaseWarningVisibility() const;
	//
	FSlateColor GetTypesIconColor() const;
	FSlateColor GetScriptIconColor() const;
	FSlateColor GetHeaderIconColor() const;
	//
	const FSlateBrush* GetCompilerIcon() const;
	//
	void OnVerticalScroll(float Offset);
	void OnAutoCompleteScroll(float Offset);
	void OnScriptHorizontalScroll(float Offset);
	void OnHeaderHorizontalScroll(float Offset);
	void OnInternalVerticalScroll(float Offset);
	void OnInternalAutoCompleteScroll(float Offset);
	//
	void OnInvokedSearch(bool DoSearch);
	void OnAdvanceAutoComplete(const FString &Search);
	void OnAutoComplete(const TArray<FString>&Results);
	void OnSearchSensitiveChanged(ECheckBoxState NewState);
	//
	void OnTypesTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnScriptTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnHeaderTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnSearchTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnTypesTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnScriptTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnHeaderTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnSearchTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnReplaceTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnMacroTextComitted(const FText &NewText, ETextCommit::Type CommitInfo, const FString Old);
	void OnIncludeTextComitted(const FText &NewText, ETextCommit::Type CommitInfo, const FString Old);
	//
	TSharedRef<ITableRow>OnGenerateMacros(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	TSharedRef<ITableRow>OnGenerateIncludes(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	TSharedRef<ITableRow>OnGenerateLineCounter(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	TSharedRef<ITableRow>OnGenerateAutoComplete(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	//
	void OnClickedAutoCompleteItem(TSharedPtr<FString>Item);
	void OnSelectedLineCounterItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo);
	void OnSelectedAutoCompleteItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo);
	//
	void SetScriptObject(UObject* New);
	void SetTypesText(const FText &NewText);
	void SetScriptText(const FText &NewText);
	void SetHeaderText(const FText &NewText);
	void SetLineCountList(const int32 Count);
	void SetErrorMessage(const FString &NewError);
	void SetMacrosList(const TArray<FString>&List);
	void SetScriptMacros(const TArray<FString>&List);
	void SetIncludesList(const TArray<FString>&List);
	void SetScriptIncludes(const TArray<FString>&List);
	void SetAutoCompleteList(const TArray<FString>&List);
	//
	//
	virtual void UpdateGraphNode() override;
	virtual void CreateBelowWidgetControls(TSharedPtr<SVerticalBox>MainBox) override;
	virtual void Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) override;
	//
	virtual bool IsInteractable() const override;
	virtual bool SupportsKeyboardFocus() const override;
	//
	virtual FReply OnKeyDown(const FGeometry &MyGeometry, const FKeyEvent &KeyEvent) override;
	virtual FReply OnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
	//
	//
	void UpdateDatabaseSemantics();
	void UpdateDatabaseReferences();
	void UpdateTextEditorScriptReference();
	void UpdateTextEditorScriptReference(UMagicNodeScript* Script);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////