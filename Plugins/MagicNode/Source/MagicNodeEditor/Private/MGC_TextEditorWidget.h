//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeDataTypes.h"
#include "MagicNodeEditor_Shared.h"

#include "Input/Reply.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Runtime/InputCore/Classes/InputCoreTypes.h"
#include "Runtime/Slate/Public/Widgets/Text/SMultiLineEditableText.h"
#include "Runtime/Slate/Public/Widgets/Text/SlateEditableTextLayout.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE(FOnInvokeSearchEvent);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ITextLayoutMarshaller;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SMGC_TextEditorWidget : public SMultiLineEditableText {
protected:
	FOnInvokeSearchEvent OnInvokedSearch;
	FOnAutoCompleteEvent OnAutoCompleted;
	FOnCursorMoved OnMovedCursor;
	//
	TArray<FString>AutoCompleteResults;
	FTextLocation CursorLocation;
	FString UnderCursor;
	bool KeyboardFocus;
	//
	void AutoCleanup(FString &Keyword);
	void AutoCompleteWord(const int32 X);
	virtual FReply OnKeyChar(const FGeometry &Geometry, const FCharacterEvent &CharacterEvent) override;
public:
	SLATE_BEGIN_ARGS(SMGC_TextEditorWidget)
	{}//
		SLATE_ARGUMENT(TSharedPtr<SMultiLineEditableText>,LineCounter)
		SLATE_ARGUMENT(TSharedPtr<ITextLayoutMarshaller>,Marshaller)
		SLATE_ARGUMENT(TSharedPtr<SScrollBar>,HScrollBar)
		SLATE_ARGUMENT(TSharedPtr<SScrollBar>,VScrollBar)
		SLATE_ATTRIBUTE(bool,CanKeyboardFocus)
		SLATE_ATTRIBUTE(bool,IsReadOnly)
		SLATE_ATTRIBUTE(FText,Text)
		//
		SLATE_EVENT(FOnAutoCompleteEvent,OnAutoComplete)
		SLATE_EVENT(FOnInvokeSearchEvent,OnInvokeSearch)
		SLATE_EVENT(FOnTextCommitted,OnTextCommitted)
		SLATE_EVENT(FOnTextChanged,OnTextChanged)
		SLATE_EVENT(FOnCursorMoved,OnCursorMoved)
	SLATE_END_ARGS()
	//
	//
	void Construct(const FArguments &InArgs);
	virtual bool SupportsKeyboardFocus() const override {return KeyboardFocus;}
	virtual FReply OnKeyDown(const FGeometry &Geometry, const FKeyEvent &KeyEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry &Geometry, const FPointerEvent &MouseEvent) override;
	virtual void Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) override;
	//
	void OnTextCursorMoved(const FTextLocation &NewPosition);
	//
	//
	void SelectLine();
	void DeleteSelectedText();
	void GoToLineColumn(int32 Line, int32 Column);
	//
	const FTextLocation &GetCursorLocation() const;
	const FString ParseAutoCompleteWord();
	const FString GetUnderCursor() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////