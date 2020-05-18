//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_TextEditorWidget.h"

#include "KMGC_TextSyntaxHighlighter.h"
#include "KMGC_ScriptParser.h"
#include "KMGC_NodeStyle.h"

#include "MagicNodeRuntime.h"

#include "Widgets/Text/SlateEditableTextLayout.h"
#include "Editor/EditorStyle/Public/EditorStyle.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::Construct(const FArguments &InArgs) {
	SuggestPicked = INDEX_NONE;
	SuggestDrawID = 0;
	//
	VScroll = InArgs._VScrollBar;
	Marshall = InArgs._Marshaller;
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
	//
	FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	LineHeight = FontMeasure->Measure("MGC_BlockStyle",FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font).Y;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int32 SKMGC_TextEditorWidget::OnPaint(const FPaintArgs &Args, const FGeometry &Geometry, const FSlateRect &CullingRect, FSlateWindowElementList &OutDrawElements, int32 LayerID, const FWidgetStyle &WidgetStyle, bool ParentEnabled) const {
	const ESlateDrawEffect DrawEffects = ESlateDrawEffect::None;
	const float AllotedHeight = Geometry.GetLocalSize().Y;
	const float AllotedWidth = Geometry.GetLocalSize().X;
	//
	//
	if (!bIsReadOnly.Get()) {
		for (int32 L=0; L<=(CountLines()); L++) {
			FSlateDrawElement::MakeBox(
				OutDrawElements,LayerID,
				Geometry.ToPaintGeometry(FVector2D(0,(LineHeight*L)-EditableTextLayout->GetScrollOffset().Y),FVector2D(AllotedWidth,LineHeight)),
				FKMGC_NodeStyle::Get().Get()->GetBrush("KMGC.Lines"),
				DrawEffects,GetLineIndexColor(L)
			);//
		} LayerID++;
	}///
	//
	//
	LayerID = SMultiLineEditableText::OnPaint(Args,Geometry,CullingRect,OutDrawElements,LayerID,WidgetStyle,ParentEnabled); LayerID++;
	//
	//
	if (!bIsReadOnly.Get()) {
		FSlateDrawElement::MakeBox(
			OutDrawElements,LayerID,
			Geometry.ToPaintGeometry(FVector2D(0,(LineHeight*CursorLocation.GetLineIndex())-EditableTextLayout->GetScrollOffset().Y),FVector2D(AllotedWidth,LineHeight)),
			FKMGC_NodeStyle::Get().Get()->GetBrush("KMGC.Focus"),
			DrawEffects,FLinearColor(0.1f,0.5f,1.f,0.45f)
		);//
		//
		LayerID++;
		//
		if (HasSuggestion()) {
			FVector2D BoxSize = GetCompletionBoxSize();
			FVector2D BoxPos = GetCompletionBoxPos();
			//
			FVector2D BorderPos = FVector2D(BoxPos.X-4,BoxPos.Y-4);
			FVector2D BorderSize = FVector2D(BoxSize.X+8,BoxSize.Y+8);
			//
			FSlateDrawElement::MakeBox(
				OutDrawElements,LayerID,
				Geometry.ToPaintGeometry(BorderPos,BorderSize),
				FEditorStyle::GetBrush("Graph.Node.Body"),
				DrawEffects,FLinearColor::White
			);//
			//
			FSlateDrawElement::MakeBox(
				OutDrawElements,LayerID,
				Geometry.ToPaintGeometry(BoxPos,BoxSize),
				FEditorStyle::GetBrush("Menu.Background"),
				DrawEffects,FLinearColor::White
			);//
			//
			for (int32 I=SuggestDrawID; I<(SuggestDrawID+MAX_SUGGESTIONS); ++I) {
				if (SuggestionResults.IsValidIndex(I)) {
					const float LineFraction = LineHeight/1.5f;
					//
					FVector2D TextPos = BoxPos;
					TextPos.Y += LineHeight * (I-SuggestDrawID);
					FVector2D Position = FVector2D(TextPos.X+(LineFraction/4),TextPos.Y+(LineFraction/4));
					//
					FSlateDrawElement::MakeBox(
						OutDrawElements,LayerID,
						Geometry.ToPaintGeometry(Position,FVector2D(LineFraction,LineFraction)),
						GetSuggestionIcon(SuggestionResults[I]),DrawEffects,
						GetSuggestionColor(SuggestionResults[I])
					);//
					//
					TextPos.X += (LineHeight*1.5f);
					FSlateDrawElement::MakeText(
						OutDrawElements,LayerID,
						Geometry.ToPaintGeometry(TextPos,FVector2D(BoxSize.X,LineHeight)),
						SuggestionResults[I],FCoreStyle::GetDefaultFontStyle("Bold",12),
						DrawEffects,GetSuggestionColor(SuggestionResults[I])
					);//
				}///
			}///
			//
			BoxPos.Y += (LineHeight*SuggestPicked);
			//
			if (SuggestPicked>INDEX_NONE) {
				FSlateDrawElement::MakeBox(
					OutDrawElements,LayerID,
					Geometry.ToPaintGeometry(BoxPos,FVector2D(BoxSize.X,LineHeight)),
					FKMGC_NodeStyle::Get().Get()->GetBrush("KMGC.Lines"),
					DrawEffects,FLinearColor(0.85f,0.95f,1.f,.45f)
				);//
			}///
		}///
		//
		if (!KeywordInfo.IsEmpty()) {
			FVector2D BoxSize = GetCompletionBoxSize();
			FVector2D BoxPos = GetCompletionBoxPos();
			//
			FVector2D FontSize = FontMeasure->Measure(KeywordInfo,FCoreStyle::GetDefaultFontStyle("Default",11));
			FVector2D BoxOffset = FVector2D(BoxPos.X+BoxSize.X+6,BoxPos.Y); FontSize.X *= 1.2f;
			FVector2D BorderSize = FVector2D(FontSize.X,FontSize.Y+4);
			//
			FSlateDrawElement::MakeBox(
				OutDrawElements,LayerID,
				Geometry.ToPaintGeometry(FVector2D(BoxOffset.X,((CursorLocation.GetLineIndex()*LineHeight)-EditableTextLayout->GetScrollOffset().Y)+(LineHeight*(SuggestPicked+1))),BorderSize),
				FEditorStyle::GetBrush("Menu.Background"),
				DrawEffects,FLinearColor::White
			);//
			//
			FSlateDrawElement::MakeText(
				OutDrawElements,LayerID,
				Geometry.ToPaintGeometry(FVector2D(BoxOffset.X+4,((CursorLocation.GetLineIndex()*LineHeight)-EditableTextLayout->GetScrollOffset().Y)+(LineHeight*(SuggestPicked+1.1f))),FontSize),
				KeywordInfo,
				FCoreStyle::GetDefaultFontStyle("Default",11),
				DrawEffects,FLinearColor::White
			);//
		}///
	}///
	//
	//
	return LayerID;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) {
	EditableTextLayout->Tick(AllottedGeometry,CurrentTime,DeltaTime);
	LastTickGeometry = AllottedGeometry;
	//
	TSharedPtr<const IRun>Run = EditableTextLayout->GetRunUnderCursor();
	if (Run.IsValid()&&CursorLocation.IsValid()&&GetSelectedText().IsEmpty()) {
		UnderCursor.Empty(); Run->AppendTextTo(UnderCursor);
	}///
	//
	if (!CursorLocation.IsValid()) {return;}
	if (UnderCursor.TrimStartAndEnd().IsEmpty()||IsOperator(UnderCursor[0])) {
		FTextLocation Offset = CursorLocation;
		int32 I = CursorLocation.GetOffset()-1;
		TCHAR CH = TEXT('_'); FString Raw;
		//
		while ((I>INDEX_NONE)&&(!(TChar<WIDECHAR>::IsWhitespace(CH)||TChar<WIDECHAR>::IsLinebreak(CH)))&&(TChar<WIDECHAR>::IsAlpha(CH)||TChar<WIDECHAR>::IsDigit(CH)||CH==TEXT('_'))) {
			Offset = FTextLocation(CursorLocation.GetLineIndex(),I);
			CH = EditableTextLayout->GetCharacterAt(Offset);
			Raw.AppendChar(CH);
		--I;}
		//
		UnderCursor = Raw.TrimStartAndEnd();
		UnderCursor.ReverseString();
		AutoCleanup(UnderCursor);
	}///
}

void SKMGC_TextEditorWidget::OnTextCursorMoved(const FTextLocation &NewPosition) {
	const float DPIScale = FSlateApplication::Get().FindWidgetWindow(this->AsShared())->GetNativeWindow()->GetDPIScaleFactor();
	CursorLocation = NewPosition;
	//
	FVector2D OutSize, OutPosition;
	FTextRange Range = FTextRange(CursorLocation.GetLineIndex(),CursorLocation.GetOffset());
	EditableTextLayout->GetTextInputMethodContext()->GetTextBounds(Range.BeginIndex,CursorLocation.GetOffset(),OutPosition,OutSize);
	//
	CompletionBoxPos = FVector2D(OutSize.X,((CursorLocation.GetLineIndex()*LineHeight)-EditableTextLayout->GetScrollOffset().Y)+LineHeight);
	CompletionBoxPos.X /= DPIScale;
	//
	//
	if ((!LockedSuggestion)&&(HasSuggestion())) {
		if (AutoCompleteKeyword.TrimStartAndEnd().IsEmpty()||(!AutoCompleteKeyword.Contains(UnderCursor))) {
			SuggestionResults.Empty(); KeywordInfo.Empty();
		}///
	} else if (!HasSuggestion()) {LockedSuggestion=false;}
	//
	OnMovedCursor.ExecuteIfBound(NewPosition);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::GoToLineColumn(int32 Line, int32 Column) {
	FTextLocation Location(Line,Column);
	//
	ScrollTo(Location);
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

const FLinearColor SKMGC_TextEditorWidget::GetLineIndexColor(int32 Line) const {
	if (Line % 2 == 0) {
		return FLinearColor(0.1f,1.f,0.5f,0.01f);
	}///
	//
	return FLinearColor(0.1f,0.5f,1.f,0.05f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FReply SKMGC_TextEditorWidget::OnKeyChar(const FGeometry &Geometry, const FCharacterEvent &CharacterEvent) {
	const TCHAR CH = CharacterEvent.GetCharacter();
	FReply Reply = FReply::Unhandled();
	//
	if (CH==TEXT('\n')||CH==TEXT('\r')) {
		if (!GetText().ToString().IsEmpty()) {
			return FReply::Handled();
		}///
	}///
	//
	if (TChar<WIDECHAR>::IsWhitespace(CH)) {
		if (!IsTextReadOnly()) {
			if (CharacterEvent.IsCommandDown()||CharacterEvent.IsControlDown()) {
				Reply = FReply::Handled();
			} else {
				EditableTextLayout->BeginEditTransation();
				//
				FString String;
				String.AppendChar(CH);
				InsertTextAtCursor(String);
				//
				EditableTextLayout->EndEditTransaction();
				//
				AutoCompleteResults.Empty();
				OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
			} Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else if (CH==TEXT('\t')) {
		if (!IsTextReadOnly()&&GetSelectedText().IsEmpty()) {
			EditableTextLayout->BeginEditTransation();
			//
			FString TAB;
			TAB.AppendChar(CH);
			InsertTextAtCursor(TAB);
			//
			EditableTextLayout->EndEditTransaction();
			//
			Reply = FReply::Handled();
		} else {Reply=FReply::Unhandled();}
	} else {Reply=SMultiLineEditableText::OnKeyChar(Geometry,CharacterEvent);}
	//
	return Reply;
}

FReply SKMGC_TextEditorWidget::OnKeyDown(const FGeometry &Geometry, const FKeyEvent &KeyEvent) {
	if (IsTextReadOnly()) {return FReply::Unhandled();}
	//
	FKey Key = KeyEvent.GetKey();
	//
	if ((Key==EKeys::F)&&(KeyEvent.IsCommandDown()||KeyEvent.IsControlDown())) {
		OnInvokedSearch.ExecuteIfBound(true);
		//
		return FReply::Handled();
	}///
	//
	if ((Key==EKeys::Up)&&HasSuggestion()) {
		SuggestPicked -= 1;
		//
		if (SuggestPicked<0) {
			SuggestDrawID = FMath::Clamp(SuggestDrawID-1,0,SuggestionResults.Num()-1);
		} SuggestPicked = FMath::Clamp(SuggestPicked,0,FMath::Min<int32>(SuggestionResults.Num()-1,MAX_SUGGESTIONS));
		//
		GetKeywordInfo();
		//
		return FReply::Handled();
	}///
	//
	if ((Key==EKeys::Down)&&HasSuggestion()) {
		SuggestPicked += 1;
		//
		/*if (SuggestPicked>(MAX_SUGGESTIONS-1)) {
			SuggestDrawID = FMath::Clamp(SuggestDrawID+1,0,(SuggestionResults.Num()-1)-MAX_SUGGESTIONS);
		}*/ SuggestPicked = FMath::Clamp(SuggestPicked,0,FMath::Min<int32>(SuggestionResults.Num()-1,(MAX_SUGGESTIONS-1)));
		//
		GetKeywordInfo();
		//
		return FReply::Handled();
	}///
	//
	if (Key==EKeys::Tab) {
		SuggestionResults.Empty();
		KeywordInfo.Empty();
		//
		FString Selected;
		//
		Selected.AppendChar(TEXT('\t'));
		if (GetSelectedText().IsEmpty()) {
			EditableTextLayout->BeginEditTransation();
			//
			Selected.Append(GetSelectedText().ToString());
			Selected.ReplaceInline(TEXT("\n"),TEXT("\n\t"));
			Selected.RemoveFromEnd(TEXT("\t"));
			Selected.AppendChar(TEXT('\n'));
			//
			InsertTextAtCursor(Selected);
			//
			EditableTextLayout->EndEditTransaction();
		}///
		//
		return FReply::Handled();
	}///
	//
	if ((Key==EKeys::Enter)&&HasSuggestion()) {
		InsertPickedSuggestion();
		return FReply::Handled();
	} else if (Key==EKeys::Enter) {
		int32 Offset = CursorLocation.GetOffset();
		FString Appended(TEXT("\n"));
		FString Line;
		//
		TArray<FString>Lines;
		GetText().ToString().ParseIntoArrayLines(Lines,false);
		//
		if (Lines.IsValidIndex(CursorLocation.GetLineIndex())) {
			EditableTextLayout->BeginEditTransation();
			//
			if (Lines[CursorLocation.GetLineIndex()].StartsWith(TEXT("\t"))) {
				Line = Lines[CursorLocation.GetLineIndex()];
				int32 I = CursorLocation.GetOffset();
				//
				for (const TCHAR &CH : Line) {
					if (CH==TEXT('\t')) {Appended.AppendChar(CH);}
					if (I>Offset) {break;}
				I++;}
				//
				InsertTextAtCursor(Appended);
			} else {InsertTextAtCursor(Appended);}
			//
			EditableTextLayout->EndEditTransaction();
			//
			return FReply::Handled();
		} else {return (SMultiLineEditableText::OnKeyDown(Geometry,KeyEvent));}
	}///
	//
	if (Key==EKeys::Delete) {
		SuggestionResults.Empty();
		KeywordInfo.Empty();
		//
		TArray<FString>Lines;
		GetText().ToString().ParseIntoArrayLines(Lines,false);
		//
		if (Lines.IsValidIndex(CursorLocation.GetLineIndex())) {
			if (CursorLocation.GetOffset()>=(Lines[CursorLocation.GetLineIndex()].Len())) {
				if (EditableTextLayout->AnyTextSelected()) {
					EditableTextLayout->BeginEditTransation();
					//
					EditableTextLayout->DeleteSelectedText();
					InsertTextAtCursor(TEXT("\n"));
					//
					EditableTextLayout->EndEditTransaction();
					//
					return FReply::Handled();
				} else {return (SMultiLineEditableText::OnKeyDown(Geometry,KeyEvent));}
			} else {return (SMultiLineEditableText::OnKeyDown(Geometry,KeyEvent));}
		}///
	}///
	//
	if ((Key==EKeys::Escape||Key==EKeys::BackSpace)&&HasAutoComplete()) {
		AutoCompleteResults.Empty();
		//
		OnInvokedSearch.ExecuteIfBound(false);
		OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
		//
		return FReply::Handled();
	} else if ((Key==EKeys::Escape||Key==EKeys::BackSpace)&&HasSuggestion()) {
		SuggestionResults.Empty();
		KeywordInfo.Empty();
		//
		return FReply::Handled();
	}///
	//
	return SMultiLineEditableText::OnKeyDown(Geometry,KeyEvent);
}

FReply SKMGC_TextEditorWidget::OnKeyUp(const FGeometry &Geometry, const FKeyEvent &KeyEvent) {
	return SMultiLineEditableText::OnKeyUp(Geometry,KeyEvent);
}

FReply SKMGC_TextEditorWidget::OnMouseButtonDown(const FGeometry &Geometry, const FPointerEvent &MouseEvent) {
	OnInvokedSearch.ExecuteIfBound(false);
	//
	if (!HasSuggestion()&&(MouseEvent.GetEffectingButton()==EKeys::LeftMouseButton)&&(MouseEvent.IsCommandDown()||MouseEvent.IsControlDown())) {
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
	if (HasSuggestion()&&(IsMouseWithinCompletionBox)) {
		InsertPickedSuggestion();
		return FReply::Handled();
	} else if(HasSuggestion()&&(!IsMouseWithinCompletionBox)) {
		SuggestionResults.Empty();
		KeywordInfo.Empty();
		//
		return FReply::Handled();
	}///
	//
	return SMultiLineEditableText::OnMouseButtonDown(Geometry,MouseEvent);
}

FReply SKMGC_TextEditorWidget::OnMouseMove(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
	MousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	//
	return (SMultiLineEditableText::OnMouseMove(MyGeometry,MouseEvent));
}

FReply SKMGC_TextEditorWidget::OnMouseWheel(const FGeometry &MyGeometry, const FPointerEvent &MouseEvent) {
	if (HasSuggestion()&&(SuggestionResults.Num()>MAX_SUGGESTIONS)) {
		if (MouseEvent.GetWheelDelta()>0) {
			SuggestDrawID = FMath::Clamp(SuggestDrawID-1,0,SuggestionResults.Num()-MAX_SUGGESTIONS);
		} else {
			SuggestDrawID = FMath::Clamp(SuggestDrawID+1,0,SuggestionResults.Num()-MAX_SUGGESTIONS);
		}///
	}///
	//
	return (SMultiLineEditableText::OnMouseWheel(MyGeometry,MouseEvent));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int32 SKMGC_TextEditorWidget::CountLines() const {
	int32 Count = 0;
	//
	for (const TCHAR &CH : GetPlainText().ToString().GetCharArray()) {
		if (CH==TEXT('\n')) {Count++;}
	}///
	//
	return Count;
}

FVector2D SKMGC_TextEditorWidget::GetCompletionBoxPos() const {
	return CompletionBoxPos;
}

FVector2D SKMGC_TextEditorWidget::GetCompletionBoxSize() const {
	return CompletionBoxSize;
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
	while ((I>INDEX_NONE)&&(!(TChar<WIDECHAR>::IsWhitespace(IT)||TChar<WIDECHAR>::IsLinebreak(IT)))&&(TChar<WIDECHAR>::IsAlpha(IT)||TChar<WIDECHAR>::IsDigit(IT)
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

void SKMGC_TextEditorWidget::SetAutoCompleteSubject(const FString &Subject) {
	AutoCompleteKeyword = Subject;
	AutoCompleteResults.Empty();
	SuggestionResults.Empty();
	KeywordInfo.Empty();
	//
	IKMGC_ScriptParser::AutoComplete(Subject,AutoCompleteKeyword,AutoCompleteResults);
	//
	if (AutoCompleteResults.Num()>=1) {
		OnAutoCompleted.ExecuteIfBound(AutoCompleteResults);
	}///
}

void SKMGC_TextEditorWidget::AutoCompleteSubject(const FString &Keyword) {
	AutoCompleteResults.Empty();
	SuggestionResults.Empty();
	KeywordInfo.Empty();
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

void SKMGC_TextEditorWidget::AutoCompleteSuggestion(const TArray<FString>&Lines, const FString &Keyword) {
	SuggestionResults.Empty();
	KeywordInfo.Empty();
	//
	IKMGC_ScriptParser::AutoSuggest(Lines,Keyword,SuggestionResults);
}

void SKMGC_TextEditorWidget::AutoCleanup(FString &Keyword) {
	FString Clean;
	//
	for (const TCHAR &CH : Keyword) {
		if (TChar<WIDECHAR>::IsAlpha(CH)||TChar<WIDECHAR>::IsDigit(CH)||CH==TEXT('_')) {
			Clean.AppendChar(CH);
		}///
	}///
	//
	Clean.ReplaceCharInline(TEXT('('),TEXT(' '));
	Clean.ReplaceCharInline(TEXT(')'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('['),TEXT(' '));
	Clean.ReplaceCharInline(TEXT(']'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('{'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('}'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('<'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('>'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT(':'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('.'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT(','),TEXT(' '));
	Clean.ReplaceCharInline(TEXT(';'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('-'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('+'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('='),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('&'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('/'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('?'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('!'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('*'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('%'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('#'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('|'),TEXT(' '));
	Clean.ReplaceCharInline(TEXT('\\'),TEXT(' '));
	Clean.RemoveSpacesInline();
	//
	Keyword.Empty(); Keyword.Append(Clean);
}

const bool SKMGC_TextEditorWidget::IsAutoComplete(const FString &Keyword) const {
	return (Keyword.EndsWith(TEXT("::"))||Keyword.EndsWith(TEXT("->"))||Keyword.EndsWith(TEXT(".")));
}

const bool SKMGC_TextEditorWidget::IsOperator(const TCHAR &CH) const {
	return (
		CH==TEXT('(') ||
		CH==TEXT(')') ||
		CH==TEXT('[') ||
		CH==TEXT(']') ||
		CH==TEXT('{') ||
		CH==TEXT('}') ||
		CH==TEXT('<') ||
		CH==TEXT('>') ||
		CH==TEXT(':') ||
		CH==TEXT('.') ||
		CH==TEXT(',') ||
		CH==TEXT(';') ||
		CH==TEXT('-') ||
		CH==TEXT('+') ||
		CH==TEXT('=') ||
		CH==TEXT('&') ||
		CH==TEXT('/') ||
		CH==TEXT('?') ||
		CH==TEXT('!') ||
		CH==TEXT('*') ||
		CH==TEXT('%') ||
		CH==TEXT('#') ||
		CH==TEXT('|') ||
		CH==TEXT('\\')
	);//
}

const bool SKMGC_TextEditorWidget::HasAutoComplete() const {
	return (AutoCompleteResults.Num()>0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::AutoSuggest(const TArray<FString>&Lines) {
	FString Subject = (UnderCursor.IsEmpty()) ? ParseAutoCompleteWord(Lines,false) : UnderCursor;
	AutoCompleteKeyword = Subject; AutoCleanup(AutoCompleteKeyword);
	//
	if (Subject.TrimStartAndEnd().IsEmpty()) {
		SuggestionResults.Empty();
		KeywordInfo.Empty();
	} else {AutoCompleteSuggestion(Lines,Subject);}
	//
	if (HasSuggestion()) {
		CompletionBoxSize = FVector2D::ZeroVector;
		CompletionBoxSize.Y = FMath::Clamp(SuggestionResults.Num()*LineHeight,0.f,LineHeight*MAX_SUGGESTIONS);
		//
		for (auto &Suggestion : SuggestionResults) {
			const float Width = FontMeasure->Measure(Suggestion,FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.CodeBlockStyle").Font).X + MAX_SUGGESTIONS;
			if (CompletionBoxSize.X<MIN_BOX_SIZE){CompletionBoxSize.X=MIN_BOX_SIZE;}
			if (Width>CompletionBoxSize.X){CompletionBoxSize.X=Width;}
		}///
		//
		if (SuggestPicked>SuggestionResults.Num()-1) {
			SuggestPicked = SuggestionResults.Num()-1;
		}///
	} else {CompletionBoxSize=FVector2D::ZeroVector;}
}

const bool SKMGC_TextEditorWidget::HasSuggestion() const {
	return ((SuggestionResults.Num()>0)&&(AutoCompleteKeyword.Len()>2));
}

void SKMGC_TextEditorWidget::InsertPickedSuggestion() {
	LockedSuggestion = true;
	//
	if (SuggestPicked>SuggestionResults.Num()-1) {
		SuggestPicked = SuggestionResults.Num()-1;
	}///
	//
	if (AutoCompleteKeyword.TrimStartAndEnd().IsEmpty()) {
		FTextLocation Offset = CursorLocation;
		int32 I = CursorLocation.GetOffset()-1;
		TCHAR CH = TEXT('a'); FString Raw;
		//
		while ((I>INDEX_NONE)&&(!(TChar<WIDECHAR>::IsWhitespace(CH)||TChar<WIDECHAR>::IsLinebreak(CH)))&&(TChar<WIDECHAR>::IsAlpha(CH)||TChar<WIDECHAR>::IsDigit(CH)||CH==TEXT('_'))) {
			Offset = FTextLocation(CursorLocation.GetLineIndex(),I);
			CH = EditableTextLayout->GetCharacterAt(Offset);
			Raw.AppendChar(CH);
		--I;}
		//
		AutoCompleteKeyword = Raw.TrimStartAndEnd();
		AutoCompleteKeyword.ReverseString();
	}///
	//
	int32 Offset = (CursorLocation.GetOffset()-(AutoCompleteKeyword.Len()+1));
	if (Offset<=INDEX_NONE) {Offset=0;}
	//
	EditableTextLayout->ClearSelection();
	if (SuggestionResults.IsValidIndex(SuggestPicked)) {
		EditableTextLayout->BeginEditTransation();
		//
		GoToLineColumn(CursorLocation.GetLineIndex(),Offset);
		EditableTextLayout->JumpTo(ETextLocation::EndOfLine,ECursorAction::SelectText);
		EditableTextLayout->DeleteSelectedText();
		//
		InsertTextAtCursor(SuggestionResults[SuggestPicked]);
		EditableTextLayout->EndEditTransaction();
	}///
	//
	SuggestionResults.Empty();
	KeywordInfo.Empty();
}

void SKMGC_TextEditorWidget::GetKeywordInfo() {
	if (HasSuggestion()&&(SuggestionResults.IsValidIndex(SuggestPicked))) {
		const FString &Keyword = SuggestionResults[SuggestPicked];
		const FKeywordDefinition &KeyInfo = IKMGC_ScriptParser::GetKeywordInfo(Keyword);
		//
		if (!KeyInfo.Info.IsEmpty()) {
			KeywordInfo = Keyword+(FString(TEXT(" ::\n\n"))+KeyInfo.Info);
		} else {
			bool Nill = true;
			KeywordInfo = Keyword+(FString(TEXT(" :: ")));
			//
			const FClassDefinition &ClassInfo = IKMGC_ScriptParser::GetClassPointerInfo(Keyword);
			FString Info = Keyword+(FString(TEXT(" ::\n\n")));
			//
			if (!ClassInfo.Hint.IsEmpty()) {Info+=(ClassInfo.Hint+FString(TEXT("\n\n"))); Nill=false;}
			if (!ClassInfo.Tooltip.IsEmpty()) {Info+=(ClassInfo.Tooltip+FString(TEXT("\n"))); Nill=false;}
			if (!ClassInfo.ParentClass.IsEmpty()) {Info+=(FString(TEXT("Parent Class: \t"))+ClassInfo.ParentClass+FString(TEXT("\n\n"))); Nill=false;}
			//
			if (!Nill) {KeywordInfo=Info;} else if (ScriptObject.IsValid()) {
				const FPropertyDefinition &PropInfo = IKMGC_ScriptParser::GetPropertyInfo(ScriptObject->GetRuntimeScriptClass(),Keyword);
				//
				if (!PropInfo.Hint.IsEmpty()) {Info+=(PropInfo.Hint+FString(TEXT("\n\n"))); Nill=false;}
				if (!PropInfo.TypeToString().Contains("?")) {Info+=(FString(TEXT("Type: \t"))+PropInfo.TypeToString()+FString(TEXT("\n"))); Nill=false;}
				if (!PropInfo.AccessToString().Contains("?")) {Info+=(FString(TEXT("Access: \t"))+PropInfo.AccessToString()+FString(TEXT("\n\n"))); Nill=false;}
				if (!PropInfo.Tooltip.IsEmpty()) {Info+=(PropInfo.Tooltip+FString(TEXT("\n"))); Nill=false;}
				//
				if (!Nill) {KeywordInfo=Info;} else {
					const FFunctionDefinition &FunInfo = IKMGC_ScriptParser::GetFunctionInfo(ScriptObject->GetRuntimeScriptClass(),Keyword);
					//
					if (!FunInfo.Hint.IsEmpty()) {Info+=(FunInfo.Hint+FString(TEXT("\n\n"))); Nill=false;}
					if (!FunInfo.TypeToString().Contains("?")) {Info+=(FString(TEXT("Type: \t"))+FunInfo.TypeToString()+FString(TEXT("\n"))); Nill=false;}
					if (!FunInfo.AccessToString().Contains("?")) {Info+=(FString(TEXT("Access: \t"))+FunInfo.AccessToString()+FString(TEXT("\n\n"))); Nill=false;}
					//
					if (!FunInfo.ReturnType.IsEmpty()) {Info+=(FString(TEXT("Return: \t"))+FunInfo.ReturnType+FString(TEXT("\n\n"))); Nill=false;}
					if (FunInfo.Inputs.Num()>=1) {
						Info+=(FString(TEXT("Inputs:\n"))); Nill=false;
						for (const FString &Input : FunInfo.Inputs) {
							Info += (FString(TEXT("\t"))+Input+FString(TEXT("\n")));
						} Info += FString(TEXT("\n"));
					}///
					//
					if (FunInfo.Outputs.Num()>=1) {
						Info+=(FString(TEXT("Outputs:\n"))); Nill=false;
						for (const FString &Output : FunInfo.Outputs) {
							Info += (FString(TEXT("\t"))+Output+FString(TEXT("\n")));
						} Info += FString(TEXT("\n"));
					}///
					//
					if (Info.Contains(TEXT("Return"))) {Info+=FString(TEXT("\n"));}
					if (!FunInfo.Tooltip.IsEmpty()) {Info+=(FunInfo.Tooltip+FString(TEXT("\n"))); Nill=false;}
					//
					if (!Nill) {KeywordInfo=Info;}
				}///
			}///
		}///
	} else {KeywordInfo.Empty();}
}

const FSlateBrush* SKMGC_TextEditorWidget::GetSuggestionIcon(const FString &Keyword) const {
	const FSlateBrush* Brush = FEditorStyle::GetBrush(TEXT("Kismet.VariableList.TypeIcon"));
	//
	if (HasSuggestion()) {
		const FKeywordDefinition &KeyInfo = IKMGC_ScriptParser::GetKeywordInfo(Keyword);
		if (KeyInfo!=IKMGC_ScriptParser::NOKeywordInfo) {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.VariableNotUsed")); return Brush;}
		//
		const FClassDefinition &ClassInfo = IKMGC_ScriptParser::GetClassPointerInfo(Keyword);
		if (ClassInfo!=IKMGC_ScriptParser::NOClassInfo) {Brush=FEditorStyle::GetBrush(TEXT("CodeView.ClassIcon")); return Brush;}
		//
		if (ScriptObject.IsValid()) {
			const FFunctionDefinition &FunInfo = IKMGC_ScriptParser::GetFunctionInfo(ScriptObject->GetRuntimeScriptClass(),Keyword);
			if (FunInfo!=IKMGC_ScriptParser::NOFunctionInfo) {Brush=FEditorStyle::GetBrush(TEXT("Kismet.AllClasses.FunctionIcon")); return Brush;}
			//
			const FPropertyDefinition &PropInfo = IKMGC_ScriptParser::GetPropertyInfo(ScriptObject->GetRuntimeScriptClass(),Keyword);
			if (PropInfo!=IKMGC_ScriptParser::NOPropertyInfo) {
				switch (PropInfo.StackOf) {
					case EStack::Set: {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.SetTypeIcon"));} break;
					case EStack::Array: {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.ArrayTypeIcon"));} break;
					case EStack::Map: {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.MapValueTypeIcon"));} break;
				default: break;}
			}///
		}///
	}///
	//
	return Brush;
}

const FLinearColor SKMGC_TextEditorWidget::GetSuggestionColor(const FString &Keyword) const {
	FLinearColor Color = FLinearColor::White;
	//
	if (HasSuggestion()) {
		const FPropertyDefinition &PropInfo = IKMGC_ScriptParser::GetPropertyInfo(ScriptObject->GetRuntimeScriptClass(),Keyword);
		if (PropInfo!=IKMGC_ScriptParser::NOPropertyInfo) {
			switch (PropInfo.TypeOf) {
				case EType::Bool: {Color=FLinearColor(0.7f,0,0);} break;
				case EType::Float: {Color=FLinearColor(0,0.8f,0);} break;
				case EType::Int: {Color=FLinearColor(0,0.95f,0.5f);} break;
				case EType::Byte: {Color=FLinearColor(0,0.4f,0.25f);} break;
				case EType::String: {Color=FLinearColor(0.85f,0,0.6f);} break;
				case EType::Name: {Color=FLinearColor(0.55f,0.35f,0.8f);} break;
				case EType::Text: {Color=FLinearColor(0.85f,0.4f,0.55f);} break;
				//
				case EType::Struct: {Color=FLinearColor(0,0,0.8f);} break;
				case EType::Enum: {Color=FLinearColor(0,0.8f,0.8f);} break;
				case EType::Class: {Color=FLinearColor(0.85f,0.35f,0.65f);} break;
				case EType::Object: {Color=FLinearColor(0.45f,0.45f,0.85f);} break;
			default: break;}
			//
			switch (PropInfo.StackOf) {
				case EStack::Set: {Color=FLinearColor(0.45f,0.85f,0.55f);} break;
				case EStack::Map: {Color=FLinearColor(0.85f,0.55f,0.45f);} break;
				case EStack::Array: {Color=FLinearColor(0.55f,0.45f,0.85f);} break;
			default: break;}
		}///
	}///
	//
	return Color;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SKMGC_TextEditorWidget::SetScriptObject(UMagicNodeScript* Script) {
	ScriptObject = Script;
}

void SKMGC_TextEditorWidget::BeginEditTransaction() {
	if (!EditableTextLayout.IsValid()) {return;}
	//
	EditableTextLayout->BeginEditTransation();
}

void SKMGC_TextEditorWidget::EndEditTransaction() {
	if (!EditableTextLayout.IsValid()) {return;}
	//
	EditableTextLayout->EndEditTransaction();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////