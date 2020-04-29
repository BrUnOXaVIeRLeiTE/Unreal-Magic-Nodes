//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///		Copyright 2020 (C) Bruno Xavier B. Leite
///		Based on code sample by Epic Games (C) 1998-2020 Epic Games, Inc.
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "KMGC_NodeStyle.h"
#include "KMGC_KismetTypes.h"
#include "KMGC_WhiteSpaceTextRun.h"
#include "MagicNodeKismet_Shared.h"

#include "Styling/SlateTypes.h"
#include "Framework/Text/SyntaxTokenizer.h"
#include "Framework/Text/SyntaxHighlighterTextLayoutMarshaller.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class FTextLayout;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class MAGICNODEKISMET_API FKMGC_TextSyntaxHighlighter : public FSyntaxHighlighterTextLayoutMarshaller {
public:
	virtual ~FKMGC_TextSyntaxHighlighter();
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
		FTextBlockStyle VariableTextStyle;
		FTextBlockStyle ProcessorTextStyle;
		//
		TArray<UMGC_SemanticDB*>SemanticDB;
		TArray<UMGC_FunctionDB*>FunctionDB;
		TArray<UMGC_KeywordDB*>KeywordDB;
		TArray<UMGC_ClassDB*>ClassDB;
		//
		FSyntaxTextStyle(const TArray<UMGC_KeywordDB*>&InKeywordDB, const TArray<UMGC_ClassDB*>&InClassDB, const TArray<UMGC_FunctionDB*>&InFunctionDB, const TArray<UMGC_SemanticDB*>&InSemanticDB)
			: TypeTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Type"))
			, MacroTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Macro"))
			, ClassTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Class"))
			, NormalTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Normal"))
			, StringTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.String"))
			, NumberTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Number"))
			, IllegalTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Illegal"))
			, KeywordTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Keyword"))
			, CommentTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Comment"))
			, OperatorTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Operator"))
			, FunctionTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Function"))
			, VariableTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Variable"))
			, ProcessorTextStyle(FKMGC_NodeStyle::Get().Get()->GetWidgetStyle<FTextBlockStyle>("KMGC.SyntaxHighlight.Processor"))
		{///
			SemanticDB = InSemanticDB;
			FunctionDB = InFunctionDB;
			KeywordDB = InKeywordDB;
			ClassDB = InClassDB;
		}///
	};//
protected:
	FKMGC_TextSyntaxHighlighter(TSharedPtr<FSyntaxTokenizer>InTokenizer, const FSyntaxTextStyle &InSyntaxTextStyle)
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
	static TSharedRef<FKMGC_TextSyntaxHighlighter>Create(const FSyntaxTextStyle &InSyntaxStyle);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////