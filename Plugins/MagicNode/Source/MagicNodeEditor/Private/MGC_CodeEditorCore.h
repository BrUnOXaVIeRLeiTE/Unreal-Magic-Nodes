//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "KMGC_TextEditorWidget.h"

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
#include "Runtime/Slate/Public/Widgets/Notifications/SNotificationList.h"

#include "Editor/EditorStyle/Public/EditorStyle.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMagicNodeScript;
class FKMGC_NodeStyle;
class UMagicNode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SMGC_CodeEditorCore : public SCompoundWidget {
private:
	TSharedPtr<ITextLayoutMarshaller>MARSHALL;
	TSharedPtr<SKMGC_TextEditorWidget>SCRIPT_EDITOR;
	TSharedPtr<SListView<TSharedPtr<FString>>>LINE_COUNTER;
	//
	TSharedPtr<SScrollBox>VS_SCROLL_BOX;
	TSharedPtr<SScrollBar>HS_SCROLL;
	TSharedPtr<SScrollBar>VS_SCROLL;
	//
	//TSharedPtr<SNotificationList>NotificationList;
	//TSharedPtr<SWidget>NavigationBar;
	//TSharedPtr<SWidget>ToolBox;
	//
	UMagicNodeScript* ScriptObject;
private:
	EMGC_CodeSource Source;
	TArray<TSharedPtr<FString>>LineCount;
	//
	TSharedPtr<FString>ExternalFilePath;
	TSharedPtr<FString>ExternalFileText;
	//
	float DatabaseLoad;
	bool RequestedUpdateDB;
public:
	SMGC_CodeEditorCore();
	~SMGC_CodeEditorCore();
	//
	SLATE_BEGIN_ARGS(SMGC_CodeEditorCore)
	{}//
		SLATE_ARGUMENT(EMGC_CodeSource,SourceToEdit)
		SLATE_ARGUMENT(FString,ExternalSourcePath)
	SLATE_END_ARGS()
	//
	//
	void Construct(const FArguments &InArgs, UMagicNodeScript* InScriptObject);
	void Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) override;
	//
	//
	//TSharedPtr<SWidget>GetToolBox() const;
	//TSharedPtr<SWidget>GetNavigationBar() const;
	//void AddNotification(FNotificationInfo &Info, bool bSuccess);
	//
	//
	bool HasScript() const;
	bool IsSourceView() const;
	bool IsScriptEditable() const;
	bool CanBuildDatabase() const;
	//
	int32 GetLineCount() const;
	FText GetScriptText() const;
	//
	void OnVerticalScroll(float Offset);
	void SetLineCountList(const int32 Count);
	void SetScriptText(const FText &NewText);
	void OnInternalVerticalScroll(float Offset);
	void OnScriptHorizontalScroll(float Offset);
	void OnScriptTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnScriptTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
	void OnSelectedLineCounterItem(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo);
	TSharedRef<ITableRow>OnGenerateLineCounter(TSharedPtr<FString>Item, const TSharedRef<STableViewBase>&OwnerTable);
	//
	//
	FText GetDatabaseLoadTooltip() const;
	TOptional<float>GetDatabaseLoad() const;
	EVisibility GetDatabaseWarningVisibility() const;
	//
	//
	void UpdateDatabaseSemantics();
	void UpdateDatabaseReferences();
	void UpdateTextEditorScriptReference();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////