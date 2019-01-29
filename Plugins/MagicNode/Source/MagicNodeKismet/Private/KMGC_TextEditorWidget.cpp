//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_TextEditorWidget.h"

#include "KMGC_TextSyntaxHighlighter.h"
#include "KMGC_ScriptParser.h"
#include "KMGC_NodeStyle.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::Construct(const FArguments &InArgs) {
	check(InArgs._Marshaller.Get());
	//
	OnAutoCompleted = InArgs._OnAutoComplete;
	OnInvokedSearch = InArgs._OnInvokeSearch;
	KeyboardFocus = InArgs._CanKeyboardFocus.Get();
	//
	SMultiLineEditableText::Construct(
		SMultiLineEditableText::FArguments()
		.TextStyle(&FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle"))
		.Font(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font)
		.OnCursorMoved(this,&SKMGC_TextEditorWidget::OnTextCursorMoved)
		.OnTextCommitted(InArgs._OnTextCommitted)
		.OnTextChanged(InArgs._OnTextChanged)
		.HScrollBar(InArgs._HScrollBar)
		.VScrollBar(InArgs._VScrollBar)
		.Marshaller(InArgs._Marshaller)
		.IsReadOnly(InArgs._IsReadOnly)
		.AutoWrapText(false)
		.Text(InArgs._Text)
		.Margin(0.f)
	);//
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) {
	EditableTextLayout->Tick(AllottedGeometry,CurrentTime,DeltaTime);
	//
	TSharedPtr<const IRun>Run = EditableTextLayout->GetRunUnderCursor();
	if (Run.IsValid() && CursorLocation.IsValid() && GetSelectedText().IsEmpty()) {
		FString Under; Run->AppendTextTo(Under);
		if (!Under.Equals(UnderCursor)) {UnderCursor=Under;}
	}///
}

void SKMGC_TextEditorWidget::OnTextCursorMoved(const FTextLocation &NewPosition) {
	CursorLocation = NewPosition;
	//
	OnMovedCursor.ExecuteIfBound(NewPosition);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::GoToLineColumn(int32 Line, int32 Column) {
	FTextLocation Location(Line,Column);
	//
	GoTo(Location);
}

void SKMGC_TextEditorWidget::SelectLine() {
	EditableTextLayout->JumpTo(ETextLocation::EndOfLine,ECursorAction::MoveCursor);
	EditableTextLayout->JumpTo(ETextLocation::BeginningOfLine,ECursorAction::SelectText);
}

void SKMGC_TextEditorWidget::DeleteSelectedText() {
	EditableTextLayout->DeleteSelectedText();
}

const FTextLocation & SKMGC_TextEditorWidget::GetCursorLocation() const {
	return CursorLocation;
}

const FString SKMGC_TextEditorWidget::GetUnderCursor() const {
	return UnderCursor;
}

const FString SKMGC_TextEditorWidget::GetAutoCompleteSubject() const {
	return AutoCompleteKeyword;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FReply SKMGC_TextEditorWidget::OnKeyChar(const FGeometry &Geometry, const FCharacterEvent &CharacterEvent) {
	const TCHAR CH = CharacterEvent.GetCharacter();
	FReply Reply = FReply::Unhandled();
	//
	if (TChar<WIDECHAR>::IsWhitespace(CH)) {
		if (!IsTextReadOnly()) {
			if (CharacterEvent.IsCommandDown()||CharacterEvent.IsControlDown()) {
				Reply = FReply::Handled();
			} else {
				FString String;
				String.AppendChar(CH);
				InsertTextAtCursor(String);
				//
				AutoCompleteResults.Empty();
				OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
			} Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else if (CH==TEXT('\t')) {
		if (!IsTextReadOnly()&&GetSelectedText().IsEmpty()) {
			FString TAB;
			TAB.AppendChar(CH);
			InsertTextAtCursor(TAB);
			//
			Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else {Reply=SMultiLineEditableText::OnKeyChar(Geometry,CharacterEvent);}
	//
	return Reply;
}

FReply SKMGC_TextEditorWidget::OnKeyDown(const FGeometry &Geometry, const FKeyEvent &KeyEvent) {
	if (IsTextReadOnly()) {return FReply::Unhandled();}
	FKey Key = KeyEvent.GetKey();
	//
	if ((Key==EKeys::F)&&(KeyEvent.IsCommandDown()||KeyEvent.IsControlDown())) {
		if (OnInvokedSearch.IsBound()) {
			OnInvokedSearch.Execute();
		} return FReply::Handled();
	}///
	//
	if (Key==EKeys::Tab) {
		FString Selected;
		Selected.AppendChar(TEXT('\t'));
		//
		if (!GetSelectedText().IsEmpty()) {
			Selected.Append(GetSelectedText().ToString());
			Selected.ReplaceInline(TEXT("\n"),TEXT("\n\t"));
			Selected.RemoveFromEnd(TEXT("\t"));
			Selected.AppendChar(TEXT('\n'));
			InsertTextAtCursor(Selected);
		}///
		//
		return FReply::Handled();
	}///
	//
	if ((KeyEvent.GetKey()==EKeys::Escape||KeyEvent.GetKey()==EKeys::BackSpace)&&(AutoCompleteResults.Num()>=1)) {
		AutoCompleteResults.Empty();
		OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
		//
		return FReply::Handled();
	}///
	//
	if (Key==EKeys::Enter) {return FReply::Handled();}
	return SMultiLineEditableText::OnKeyDown(Geometry,KeyEvent);
}

FReply SKMGC_TextEditorWidget::OnMouseButtonDown(const FGeometry &Geometry, const FPointerEvent &MouseEvent) {
	FReply Reply = SMultiLineEditableText::OnMouseButtonDown(Geometry,MouseEvent);
	//
	if ((MouseEvent.GetEffectingButton()==EKeys::LeftMouseButton)&&(MouseEvent.IsCommandDown()||MouseEvent.IsControlDown())) {
		EditableTextLayout->SelectWordAt(Geometry,MouseEvent.GetScreenSpacePosition());
		UnderCursor = GetSelectedText().ToString();
		//
		LOG_MGC(EMGCSeverity::Info,TEXT("Searching for Class definition..."));
		const FString IURL = IKMGC_ScriptParser::GetClassDocumentation(UnderCursor);
		//
		if (UKismetSystemLibrary::CanLaunchURL(IURL)) {
			UKismetSystemLibrary::LaunchURL(IURL);
		} else {
			LOG_MGC(EMGCSeverity::Info,FString::Printf(TEXT("Unable to locate documentation for keyword. Launching search page for:   %s"),*UnderCursor));
			const FString _Search = FString::Printf(TEXT("https://www.unrealengine.com/en-US/bing-search?keyword=%s"),*UnderCursor);
			UKismetSystemLibrary::LaunchURL(_Search);
		}///
		//
		return FReply::Handled();
	}///
	//
	return Reply;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const FString SKMGC_TextEditorWidget::ParseAutoCompleteWord(const TArray<FString>&Lines, const bool CleanUp) {
	if (!HasKeyboardFocus()) {return FString();}
	//
	FTextLocation Offset;
	if (Lines.IsValidIndex(CursorLocation.GetLineIndex())) {
		if (Lines[CursorLocation.GetLineIndex()].Len() < CursorLocation.GetOffset()) {
			CursorLocation = FTextLocation(CursorLocation.GetLineIndex(),Lines[CursorLocation.GetLineIndex()].Len()-1);
		} Offset = CursorLocation;
	} if (!Offset.IsValid()||(Offset.GetOffset()<=INDEX_NONE+1)) {return FString();}
	//
	int32 I = Offset.GetOffset();
	TCHAR IT = Lines[Offset.GetLineIndex()].GetCharArray()[Offset.GetOffset()];
	//
	FString Subject;
	FString Raw;
	//
	while ((I>INDEX_NONE)&&(!TChar<WIDECHAR>::IsWhitespace(IT))&&(!TChar<WIDECHAR>::IsLinebreak(IT))&&(TChar<WIDECHAR>::IsAlpha(IT)||TChar<WIDECHAR>::IsDigit(IT)
		||IT==TEXT('.')||IT==TEXT(':')||IT==TEXT('-')||IT==TEXT('_')||IT==TEXT('<')||IT==TEXT('>')||IT==TEXT('{')||IT==TEXT('}')||IT==TEXT('[')||IT==TEXT(']')||IT==TEXT('(')||IT==TEXT(')')
	)) {
		Offset = FTextLocation(CursorLocation.GetLineIndex(),I);
		IT = EditableTextLayout->GetCharacterAt(Offset);
		Raw.AppendChar(IT);
	--I;}
	//
	Subject = Raw.TrimStartAndEnd();
	Subject.ReverseString();
	//
	Subject.Split(TEXT("{"),&Subject,nullptr); Subject.Split(TEXT("("),&Subject,nullptr);
	Subject.Split(TEXT("["),&Subject,nullptr); Subject.Split(TEXT("<"),&Subject,nullptr);
	//
	if (CleanUp) {
		if (Subject.Contains(TEXT("::"))) {Subject.Split(TEXT("::"),nullptr,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
		if (Subject.Contains(TEXT("->"))) {Subject.Split(TEXT("->"),nullptr,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
		if (Subject.Contains(TEXT("."))) {Subject.Split(TEXT("."),nullptr,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	}///
	//
	return Subject;
}

void SKMGC_TextEditorWidget::AutoCompleteSubject(const FString &Keyword) {
	AutoCompleteResults.Empty();
	//
	FString Parent;
	FString Subject = Keyword.TrimStartAndEnd();
	//
	if (Subject.Contains(TEXT("::"))) {Subject.Split(TEXT("::"),&Parent,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	if (Subject.Contains(TEXT("->"))) {Subject.Split(TEXT("->"),&Parent,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	if (Subject.Contains(TEXT("."))) {Subject.Split(TEXT("."),&Parent,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	//
	if (Parent.Contains(TEXT("::"))) {Parent.Split(TEXT("::"),nullptr,&Parent,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	if (Parent.Contains(TEXT("->"))) {Parent.Split(TEXT("->"),nullptr,&Parent,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	if (Parent.Contains(TEXT("."))) {Parent.Split(TEXT("."),nullptr,&Parent,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	//
	Parent.Split(TEXT("{"),&Parent,nullptr); Parent.Split(TEXT("("),&Parent,nullptr);
	Parent.Split(TEXT("["),&Parent,nullptr); Parent.Split(TEXT("<"),&Parent,nullptr);
	//
	Subject.Split(TEXT("{"),&Subject,nullptr); Subject.Split(TEXT("("),&Subject,nullptr);
	Subject.Split(TEXT("["),&Subject,nullptr); Subject.Split(TEXT("<"),&Subject,nullptr);
	//
	AutoCleanup(Parent);
	AutoCleanup(Subject);
	//
	if (Subject.IsEmpty()) {
		AutoCompleteKeyword = Parent;
		IKMGC_ScriptParser::AutoComplete(Parent,AutoCompleteKeyword,AutoCompleteResults);
	} else {
		AutoCompleteKeyword = Subject;
		IKMGC_ScriptParser::AutoComplete(Parent,Subject,AutoCompleteResults);
	}///
	//
	OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
}

void SKMGC_TextEditorWidget::AutoCleanup(FString &Keyword) {
	FString Clean;
	//
	for (const TCHAR &CH : Keyword) {
		if ((TChar<WIDECHAR>::IsAlpha(CH)||TChar<WIDECHAR>::IsDigit(CH)||CH==TEXT('_'))) {Clean.AppendChar(CH);}
	}///
	//
	Keyword.Empty();
	Keyword.Append(Clean);
}

void SKMGC_TextEditorWidget::SetAutoCompleteSubject(const FString &Subject) {
	AutoCompleteResults.Empty();
	AutoCompleteKeyword = Subject;
	//
	IKMGC_ScriptParser::AutoComplete(Subject,AutoCompleteKeyword,AutoCompleteResults);
	//
	if (AutoCompleteResults.Num()>=1) {
		OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
	}///
}

const bool SKMGC_TextEditorWidget::IsAutoComplete(const FString &Keyword) const {
	return (Keyword.EndsWith(TEXT("::"))||Keyword.EndsWith(TEXT("->"))||Keyword.EndsWith(TEXT(".")));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////