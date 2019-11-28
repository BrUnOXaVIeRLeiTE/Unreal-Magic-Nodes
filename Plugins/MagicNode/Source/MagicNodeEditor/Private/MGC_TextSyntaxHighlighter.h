//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "KMGC_KismetTypes.h"

#include "MGC_CodeEditorStyle.h"
#include "MGC_WhiteSpaceTextRun.h"
#include "MagicNodeEditor_Shared.h"

#include "Styling/SlateTypes.h"
#include "Framework/Text/SyntaxTokenizer.h"
#include "Framework/Text/SyntaxHighlighterTextLayoutMarshaller.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FTextLayout;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FMGC_TextSyntaxHighlighter : public FSyntaxHighlighterTextLayoutMarshaller {
public:
	virtual ~FMGC_TextSyntaxHighlighter();
	//
	struct FSyntaxTextStyle {
		FTextBlockStyle TypeTextStyle;
		FTextBlockStyle MacroTextStyle;
		FTextBlockStyle ClassTextStyle;
		FTextBlockStyle NormalTextStyle;
		FTextBlockStyle StringTextStyle;
		FTextBlockStyle NumberTextStyle;
		FTextBlockStyle IllegalTextStyle;
		FTextBlockStyle KeywordTextStyle;
		FTextBlockStyle CommentTextStyle;
		FTextBlockStyle OperatorTextStyle;
		FTextBlockStyle FunctionTextStyle;
		FTextBlockStyle ProcessorTextStyle;
		//
		TArray<UMGC_FunctionDB*>FunctionDB;
		TArray<UMGC_KeywordDB*>KeywordDB;
		TArray<UMGC_ClassDB*>ClassDB;
		//
		FSyntaxTextStyle(const TArray<UMGC_KeywordDB*>&InKeywordDB, const TArray<UMGC_ClassDB*>&InClassDB, const TArray<UMGC_FunctionDB*>&InFunctionDB)
			: TypeTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Type"))
			, MacroTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Macro"))
			, ClassTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Class"))
			, NormalTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Normal"))
			, StringTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.String"))
			, NumberTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Number"))
			, IllegalTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Illegal"))
			, KeywordTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Keyword"))
			, CommentTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Comment"))
			, OperatorTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Operator"))
			, FunctionTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Function"))
			, ProcessorTextStyle(FMGC_CodeEditorStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("MGC.SyntaxHighlight.Processor"))
		{///
			FunctionDB = InFunctionDB;
			KeywordDB = InKeywordDB;
			ClassDB = InClassDB;
		}///
	};//
protected:
	FMGC_TextSyntaxHighlighter(TSharedPtr<FSyntaxTokenizer>InTokenizer, const FSyntaxTextStyle &InSyntaxTextStyle)
		: FSyntaxHighlighterTextLayoutMarshaller(MoveTemp(InTokenizer))
		, SyntaxTextStyle(InSyntaxTextStyle)
	{}//
	//
	enum class EParserState : uint8 {
		None,
		LookingForChar,
		LookingForString,
		LookingForMultiLineComment,
		LookingForSingleLineComment,
	};//
	//
	FSyntaxTextStyle SyntaxTextStyle;
	FString TabString;
	//
	//
	uint32 CountCharacters(const FString &Source, const TCHAR &Char) const;
	//
	virtual void ParseTokens(const FString &Source, FTextLayout &Layout, TArray<FSyntaxTokenizer::FTokenizedLine>TokenizedLines) override;
	void ParseToken(const FString &Source, const FSyntaxTokenizer::FToken &Token, const FString &Keyword, FRunInfo &Info, FTextBlockStyle &TextStyle, EParserState &ParserState, bool &LoopBack);
public:
	static TSharedRef<FMGC_TextSyntaxHighlighter>Create(const FSyntaxTextStyle &InSyntaxStyle);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////