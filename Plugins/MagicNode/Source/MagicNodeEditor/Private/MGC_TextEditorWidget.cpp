//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_TextEditorWidget.h"

#include "MGC_TextSyntaxHighlighter.h"
#include "MGC_CodeEditorStyle.h"

#include "KMGC_ScriptParser.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_TextEditorWidget::Construct(const FArguments &InArgs) {
	check(InArgs._Marshaller.Get());
	//
	OnAutoCompleted = InArgs._OnAutoComplete;
	OnInvokedSearch = InArgs._OnInvokeSearch;
	KeyboardFocus = InArgs._CanKeyboardFocus.Get();
	//
	SMultiLineEditableText::Construct(
		SMultiLineEditableText::FArguments()
		.TextStyle(&FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.CodeBlockStyle"))
		.Font(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.CodeBlockStyle").Font)
		.OnCursorMoved(this,&SMGC_TextEditorWidget::OnTextCursorMoved)
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

void SMGC_TextEditorWidget::Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) {
	EditableTextLayout->Tick(AllottedGeometry,CurrentTime,DeltaTime);
	//
	TSharedPtr<const IRun>Run = EditableTextLayout->GetRunUnderCursor();
	if (Run.IsValid() && CursorLocation.IsValid() && GetSelectedText().IsEmpty()) {
		FString Under; Run->AppendTextTo(Under);
		if (!Under.Equals(UnderCursor)) {UnderCursor=Under;}
	}///
}

void SMGC_TextEditorWidget::OnTextCursorMoved(const FTextLocation &NewPosition) {
	CursorLocation = NewPosition;
	//
	OnMovedCursor.ExecuteIfBound(NewPosition);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SMGC_TextEditorWidget::GoToLineColumn(int32 Line, int32 Column) {
	FTextLocation Location(Line,Column);
	//
	GoTo(Location);
}

void SMGC_TextEditorWidget::SelectLine() {
	EditableTextLayout->JumpTo(ETextLocation::EndOfLine,ECursorAction::MoveCursor);
	EditableTextLayout->JumpTo(ETextLocation::BeginningOfLine,ECursorAction::SelectText);
}

void SMGC_TextEditorWidget::DeleteSelectedText() {
	EditableTextLayout->DeleteSelectedText();
}

const FTextLocation & SMGC_TextEditorWidget::GetCursorLocation() const {
	return CursorLocation;
}

const FString SMGC_TextEditorWidget::GetUnderCursor() const {
	return UnderCursor;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FReply SMGC_TextEditorWidget::OnKeyChar(const FGeometry &Geometry, const FCharacterEvent &CharacterEvent) {
	const TCHAR CH = CharacterEvent.GetCharacter();
	FReply Reply = FReply::Unhandled();
	//
	if (TChar<WIDECHAR>::IsWhitespace(CH)) {
		if (!IsTextReadOnly()) {
			if ((CharacterEvent.IsCommandDown()||CharacterEvent.IsControlDown())&&((CursorLocation.GetOffset()-1)>INDEX_NONE)) {
				FString AR;
				AR.AppendChar(TEXT('-'));
				AR.AppendChar(TEXT('>'));
				//
				InsertTextAtCursor(AR);
				AutoCompleteWord(2);
			} else {
				FString String;
				String.AppendChar(CH);
				InsertTextAtCursor(String);
				//
				AutoCompleteResults.Empty();
				OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
			} Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else if (CH==TEXT('.')) {
		if (!IsTextReadOnly()) {
			if (CursorLocation.GetOffset()-1 > INDEX_NONE) {
				AutoCompleteWord(1);
			}///
			//
			FString DOT;
			DOT.AppendChar(CH);
			InsertTextAtCursor(DOT);
			//
			Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else if (CH==TEXT('>')) {
		if (!IsTextReadOnly()) {
			if (CursorLocation.GetOffset()-2 > INDEX_NONE) {
				FTextLocation Offset(CursorLocation.GetLineIndex(),CursorLocation.GetOffset()-1);
				TCHAR AR = EditableTextLayout->GetCharacterAt(Offset);
				if (AR==TEXT('-')) {AutoCompleteWord(2);}
			}///
			//
			FString BR;
			BR.AppendChar(CH);
			InsertTextAtCursor(BR);
			//
			Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else if (CH==TEXT(':')) {
		if (!IsTextReadOnly()) {
			if (CursorLocation.GetOffset()-2 > INDEX_NONE) {
				FTextLocation Offset(CursorLocation.GetLineIndex(),CursorLocation.GetOffset()-1);
				TCHAR AR = EditableTextLayout->GetCharacterAt(Offset);
				if (AR==TEXT(':')) {AutoCompleteWord(2);}
			}///
			//
			FString DOT;
			DOT.AppendChar(CH);
			InsertTextAtCursor(DOT);
			//
			Reply = FReply::Handled();
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

FReply SMGC_TextEditorWidget::OnKeyDown(const FGeometry &Geometry, const FKeyEvent &KeyEvent) {
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

FReply SMGC_TextEditorWidget::OnMouseButtonDown(const FGeometry &Geometry, const FPointerEvent &MouseEvent) {
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

void SMGC_TextEditorWidget::AutoCompleteWord(const int32 X) {
	if (CursorLocation.GetOffset()-X <= INDEX_NONE) {return;}
	//
	FTextLocation Offset(CursorLocation.GetLineIndex(),CursorLocation.GetOffset()-X);
	TCHAR IT = EditableTextLayout->GetCharacterAt(Offset);
	int32 I = Offset.GetOffset();
	AutoCompleteResults.Empty();
	//
	FString Subject;
	FString Parent;
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
	IKMGC_ScriptParser::AutoComplete(Parent,Subject,AutoCompleteResults);
	OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
}

const FString SMGC_TextEditorWidget::ParseAutoCompleteWord() {
	if (CursorLocation.GetOffset() <= INDEX_NONE+1) {return FString();}
	//
	FTextLocation Offset(CursorLocation.GetLineIndex(),CursorLocation.GetOffset());
	TCHAR IT = EditableTextLayout->GetCharacterAt(Offset);
	int32 I = Offset.GetOffset();
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
	if (Subject.Contains(TEXT("::"))) {Subject.Split(TEXT("::"),nullptr,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	if (Subject.Contains(TEXT("->"))) {Subject.Split(TEXT("->"),nullptr,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	if (Subject.Contains(TEXT("."))) {Subject.Split(TEXT("."),nullptr,&Subject,ESearchCase::IgnoreCase,ESearchDir::FromEnd);}
	//
	Subject.Split(TEXT("{"),&Subject,nullptr); Subject.Split(TEXT("("),&Subject,nullptr);
	Subject.Split(TEXT("["),&Subject,nullptr); Subject.Split(TEXT("<"),&Subject,nullptr);
	//
	AutoCleanup(Subject);
	//
	return Subject;
}

void SMGC_TextEditorWidget::AutoCleanup(FString &Keyword) {
	FString Clean;
	//
	for (const TCHAR &CH : Keyword) {
		if ((TChar<WIDECHAR>::IsAlpha(CH)||TChar<WIDECHAR>::IsDigit(CH)||CH==TEXT('_'))) {Clean.AppendChar(CH);}
	}///
	//
	Keyword.Empty();
	Keyword.Append(Clean);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////