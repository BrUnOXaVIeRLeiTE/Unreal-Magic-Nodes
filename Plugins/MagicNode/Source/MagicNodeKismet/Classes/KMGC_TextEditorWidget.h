//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeDataTypes.h"
#include "MagicNodeRuntime_Shared.h"

#include "Input/Reply.h"
#include "Fonts/FontMeasure.h"

#include "Styling/CoreStyle.h"
#include "Styling/SlateTypes.h"

#include "Runtime/SlateCore/Public/Widgets/Images/SImage.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBar.h"
#include "Runtime/Slate/Public/Widgets/Text/SMultiLineEditableText.h"
#include "Runtime/Slate/Public/Widgets/Text/SlateEditableTextLayout.h"
#include "Runtime/SlateCore/Public/Widgets/DeclarativeSyntaxSupport.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_DELEGATE_OneParam(FOnInvokeSearchEvent,bool);
DECLARE_DELEGATE_OneParam(FOnAutoCompleteEvent,const TArray<FString>&);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ITextLayoutMarshaller;
class UMagicNodeScript;
class UKMGC_MagicNode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class EAutoComplete : uint8 {
	Off,
	Active
};

enum class EAutoSuggest : uint8 {
	Word,
	Class,
	Property,
	Function
};

#define MIN_BOX_SIZE 120
#define MAX_SUGGESTIONS 20

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MAGICNODEKISMET_API SKMGC_TextEditorWidget : public SMultiLineEditableText {
private:
	TWeakObjectPtr<UMagicNodeScript>ScriptObject;
	//
	float LineHeight;
	int32 SuggestPicked;
	int32 SuggestDrawID;
	//
	bool KeyboardFocus;
	bool LockedSuggestion;
	bool IsMouseWithinCompletionBox;
protected:
	TSharedPtr<ITextLayoutMarshaller>Marshall;
	TSharedPtr<FSlateFontMeasure>FontMeasure;
	TSharedPtr<SScrollBar>VScroll;
	//
	FOnInvokeSearchEvent OnInvokedSearch;
	FOnAutoCompleteEvent OnAutoCompleted;
	//
	FOnCursorMoved OnMovedCursor;
	FTextLocation CursorLocation;
	//
	TArray<FString>AutoCompleteResults;
	TArray<FString>SuggestionResults;
	//
	FString AutoCompleteKeyword;
	FString UnderCursor;
	FString KeywordInfo;
	//
	FVector2D CompletionBoxSize;
	FVector2D CompletionBoxPos;
	FVector2D MousePosition;
	//
	FGeometry LastTickGeometry;
protected:
	void AutoCleanup(FString &Keyword);
public:
	SLATE_BEGIN_ARGS(SKMGC_TextEditorWidget)
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
	virtual void Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) override;
	virtual int32 OnPaint(const FPaintArgs &Args, const FGeometry &Geometry, const FSlateRect &CullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerID, const FWidgetStyle &WidgetStyle, bool ParentEnabled) const override;
	//
	virtual bool SupportsKeyboardFocus() const override {return KeyboardFocus;}
	//
	virtual FReply OnKeyUp(const FGeometry &Geometry, const FKeyEvent &KeyEvent) override;
	virtual FReply OnKeyDown(const FGeometry &Geometry, const FKeyEvent &KeyEvent) override;
	virtual FReply OnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) override;
	virtual FReply OnKeyChar(const FGeometry &Geometry, const FCharacterEvent &CharacterEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry &Geometry, const FPointerEvent &MouseEvent) override;
	//
	void OnTextCursorMoved(const FTextLocation &NewPosition);
	//
	void SelectLine();
	void DeleteSelectedText();
	void GoToLineColumn(int32 Line, int32 Column);
	//
	const FTextLocation &GetCursorLocation() const;
	const FString GetUnderCursor() const;
	//
	int32 CountLines() const;
	const FLinearColor GetLineIndexColor(int32 Line) const;
	//
	//
	void GetKeywordInfo();
	void InsertPickedSuggestion();
	void AutoSuggest(const TArray<FString>&Lines);
	void AutoCompleteSubject(const FString &Keyword);
	void SetAutoCompleteSubject(const FString &Subject);
	void AutoCompleteSuggestion(const TArray<FString>&Lines, const FString &Keyword);
	//
	const FString GetAutoCompleteSubject() const;
	const FString ParseAutoCompleteWord(const TArray<FString>&Lines, const bool CleanUp=false);
	//
	FVector2D GetCompletionBoxPos() const;
	FVector2D GetCompletionBoxSize() const;
	//
	const bool HasSuggestion() const;
	const bool HasAutoComplete() const;
	const bool IsOperator(const TCHAR &CH) const;
	const bool IsAutoComplete(const FString &Keyword) const;
	//
	const FSlateBrush* GetSuggestionIcon(const FString &Keyword) const;
	const FLinearColor GetSuggestionColor(const FString &Keyword) const;
public:
	void SetScriptObject(UMagicNodeScript* Script);
	//
	void BeginEditTransaction();
	void EndEditTransaction();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////