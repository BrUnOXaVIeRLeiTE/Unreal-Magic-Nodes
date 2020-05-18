//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///		Copyright 2020 (C) Bruno Xavier B. Leite
///		Based on code sample by Epic Games (C) 1998-2020 Epic Games, Inc.
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_TextSyntaxHighlighter.h"

#include "Framework/Text/IRun.h"
#include "Framework/Text/ISlateRun.h"
#include "Framework/Text/TextLayout.h"
#include "Framework/Text/SlateTextRun.h"

#include "Misc/ExpressionParserTypes.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FKMGC_TextSyntaxHighlighter::~FKMGC_TextSyntaxHighlighter(){}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<FKMGC_TextSyntaxHighlighter>FKMGC_TextSyntaxHighlighter::Create(const FSyntaxTextStyle &InSyntaxStyle) {
	TArray<FSyntaxTokenizer::FRule>TokenizerRules;
	//
	if (InSyntaxStyle.KeywordDB.Num()>0) {
		for (UMGC_KeywordDB* DB : InSyntaxStyle.KeywordDB) {
			if (DB==nullptr) {continue;}
			//
			const TSet<FString>Core = DB->ScriptCore;
			const TSet<FString>Extended = DB->Extensions;
			//
			for (const FString Keyword : Core) {
				TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Keyword));
			}///
			//
			for (const FString Keyword : Extended) {
				if (Core.Contains(Keyword)) {continue;}
				TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Keyword));
			}///
		}///
	}///
	//
	if (InSyntaxStyle.KeywordDB.Num()>0) {
		for (const UMGC_KeywordDB* DB : InSyntaxStyle.KeywordDB) {
			if (DB==nullptr) {continue;}
			//
			const TSet<FString>Operators = DB->Operators;
			//
			for (const FString Operator : Operators) {
				TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Operator));
			}///
		}///
	}///
	//
	if (InSyntaxStyle.KeywordDB.Num()>0) {
		for (const UMGC_KeywordDB* DB : InSyntaxStyle.KeywordDB) {
			if (DB==nullptr) {continue;}
			//
			const TSet<FString>Processors = DB->Processors;
			//
			for (const FString Processor : Processors) {
				TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Processor));
			}///
		}///
	}///
	//
	if (InSyntaxStyle.ClassDB.Num()>0) {
		for (const UMGC_ClassDB* DB : InSyntaxStyle.ClassDB) {
			if (DB==nullptr) {continue;}
			//
			const TSet<FString>Types = DB->ScriptTypes;
			//
			for (const FString Typed : Types) {
				TokenizerRules.Emplace(FSyntaxTokenizer::FRule(Typed));
			}///
		}///
	}///
	//
	//
	return MakeShareable(new FKMGC_TextSyntaxHighlighter(FSyntaxTokenizer::Create(TokenizerRules),InSyntaxStyle));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FKMGC_TextSyntaxHighlighter::ParseTokens(const FString &Source, FTextLayout &Layout, TArray<FSyntaxTokenizer::FTokenizedLine>TokenizedLines) {
	TArray<FTextLayout::FNewLineData>ParsedLines;
	ParsedLines.Reserve(TokenizedLines.Num());
	//
	EParserState ParserState = EParserState::None;
	//
	for (const FSyntaxTokenizer::FTokenizedLine &TokenizedLine : TokenizedLines) {
		if (ParserState==EParserState::LookingForSingleLineComment) {ParserState=EParserState::None;}
		//
		TSharedRef<FString>Runner = MakeShareable(new FString());
		TArray<TSharedRef<IRun>>Runs;
		//
		for (const FSyntaxTokenizer::FToken &Token : TokenizedLine.Tokens) {
			const FString TText = Source.Mid(Token.Range.BeginIndex,Token.Range.Len());
			const FTextRange RunRange(Runner->Len(),Runner->Len()+TText.Len());
			const bool Whitespace = FString(TText).TrimEnd().IsEmpty();
			//
			FTextBlockStyle TextStyle = SyntaxTextStyle.NormalTextStyle;
			FRunInfo RunInfo(TEXT("KMGC.SyntaxHighlight.Normal"));
			Runner->Append(TText);
			//
			if (Whitespace) {
				RunInfo.Name = TEXT("KMGC.SyntaxHighlight.WhiteSpace");
				TSharedRef<ISlateRun>Run=FWhiteSpaceTextRun::Create(RunInfo,Runner,TextStyle,RunRange,4); Runs.Add(Run);
			} else {
				bool LoopBack=false;
				ParseToken(Source,Token,TText,RunInfo,TextStyle,ParserState,LoopBack);
				TSharedRef<ISlateRun>Run=FSlateTextRun::Create(RunInfo,Runner,TextStyle,RunRange); Runs.Add(Run);
			}///
		}///
		//
		ParsedLines.Emplace(MoveTemp(Runner),MoveTemp(Runs));
	}///
	//
	Layout.AddLines(ParsedLines);
}

void FKMGC_TextSyntaxHighlighter::ParseToken(const FString &Source, const FSyntaxTokenizer::FToken &Token, const FString &Keyword, FRunInfo &Info, FTextBlockStyle &TextStyle, EParserState &ParserState, bool &LoopBack) {
	uint32 FBracket = CountCharacters(Source,TEXT('[')); uint32 BBracket = CountCharacters(Source,TEXT(']'));
	uint32 FParent = CountCharacters(Source,TEXT('(')); uint32 BParent = CountCharacters(Source,TEXT(')'));
	uint32 FBraces = CountCharacters(Source,TEXT('{')); uint32 BBraces = CountCharacters(Source,TEXT('}'));
	//
	if (Token.Type==FSyntaxTokenizer::ETokenType::Syntax) {
		if (ParserState==EParserState::None && Keyword==TEXT("//")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Comment");
			TextStyle = SyntaxTextStyle.CommentTextStyle;
			ParserState = EParserState::LookingForSingleLineComment;
		} else if (ParserState==EParserState::None && Keyword==TEXT("/*")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Comment");
			TextStyle = SyntaxTextStyle.CommentTextStyle;
			ParserState = EParserState::LookingForMultiLineComment;
		} else if (ParserState==EParserState::LookingForMultiLineComment && Keyword==TEXT("*/")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Comment");
			TextStyle = SyntaxTextStyle.CommentTextStyle;
			ParserState = EParserState::None;
		} else if (ParserState==EParserState::None && Keyword==TEXT("\"")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.String");
			TextStyle = SyntaxTextStyle.StringTextStyle;
			ParserState = EParserState::LookingForString;
			LoopBack = true;
		} else if (ParserState==EParserState::LookingForString && Keyword==TEXT("\"")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Normal");
			TextStyle = SyntaxTextStyle.StringTextStyle;
			ParserState = EParserState::None;
		} else if (ParserState==EParserState::None && Keyword==TEXT("\'")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.String");
			TextStyle = SyntaxTextStyle.StringTextStyle;
			ParserState = EParserState::LookingForChar;
			LoopBack = true;
		} else if (ParserState==EParserState::LookingForChar && Keyword==TEXT("\'")) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Normal");
			TextStyle = SyntaxTextStyle.StringTextStyle;
			ParserState = EParserState::None;
		} else if(ParserState==EParserState::None && Keyword.StartsWith(TEXT("#"))) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Processor");
			TextStyle = SyntaxTextStyle.ProcessorTextStyle;
			ParserState = EParserState::None;
		} else if (ParserState==EParserState::None && TChar<WIDECHAR>::IsAlpha(Keyword[0])) {
			for (const UMGC_KeywordDB* DB : SyntaxTextStyle.KeywordDB) {
				if (const FString*F=DB->ScriptCore.Find(Keyword)) {
					if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
						Info.Name = TEXT("KMGC.SyntaxHighlight.Keyword");
						TextStyle = SyntaxTextStyle.KeywordTextStyle;
					}///
				break;}
				//
				if (const FString*F=DB->Extensions.Find(Keyword)) {
					if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
						Info.Name = TEXT("KMGC.SyntaxHighlight.Keyword");
						TextStyle = SyntaxTextStyle.KeywordTextStyle;
					}///
				break;}
			}///
			//
			for (const UMGC_ClassDB* DB : SyntaxTextStyle.ClassDB) {
				if (const FString*F=DB->ScriptTypes.Find(Keyword)) {
					if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
						Info.Name = TEXT("KMGC.SyntaxHighlight.Type");
						TextStyle = SyntaxTextStyle.TypeTextStyle;
					}///
				break;}
			}///
			//
			ParserState = EParserState::None;
		} else if (ParserState==EParserState::None && !TChar<WIDECHAR>::IsAlpha(Keyword[0])) {
			if ((Keyword[0]==TEXT('(')||Keyword[0]==TEXT(')'))&&(FParent!=BParent)) {
				Info.Name = TEXT("KMGC.SyntaxHighlight.Illegal");
				TextStyle = SyntaxTextStyle.IllegalTextStyle;
			} else if ((Keyword[0]==TEXT('[')||Keyword[0]==TEXT(']'))&&(FBracket!=BBracket)) {
				Info.Name = TEXT("KMGC.SyntaxHighlight.Illegal");
				TextStyle = SyntaxTextStyle.IllegalTextStyle;
			} else if ((Keyword[0]==TEXT('{')||Keyword[0]==TEXT('}'))&&(FBraces!=BBraces)) {
				Info.Name = TEXT("KMGC.SyntaxHighlight.Illegal");
				TextStyle = SyntaxTextStyle.IllegalTextStyle;
			} else {
				for (const UMGC_KeywordDB* DB : SyntaxTextStyle.KeywordDB) {
					if (const FString*F=DB->Operators.Find(Keyword)) {
						if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
							Info.Name = TEXT("KMGC.SyntaxHighlight.Operator");
							TextStyle = SyntaxTextStyle.OperatorTextStyle;
						}///
					break;}
				}///
			}///
			//
			ParserState = EParserState::None;
		}///
	} else if (ParserState==EParserState::None && (Keyword.IsNumeric()||
		Keyword.Replace(TEXT("f"),TEXT("")).IsNumeric()||
		Keyword.Replace(TEXT(".f"),TEXT("")).IsNumeric())
	) {
		Info.Name = TEXT("KMGC.SyntaxHighlight.Number");
		TextStyle = SyntaxTextStyle.NumberTextStyle;
	} else if (ParserState==EParserState::None) {
		for (const UMGC_SemanticDB* DB : SyntaxTextStyle.SemanticDB) {
			for (auto IT=DB->ClassRedirectors.CreateConstIterator(); IT; ++IT) {
				if (IT->Key.Equals(Keyword,ESearchCase::CaseSensitive)) {
					Info.Name = TEXT("KMGC.SyntaxHighlight.Variable");
					TextStyle = SyntaxTextStyle.VariableTextStyle;
				}///
			break;}
		}///
		//
		for (const UMGC_FunctionDB* DB : SyntaxTextStyle.FunctionDB) {
			if (const FString*F=DB->ScriptCore.Find(Keyword)) {
				if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
					Info.Name = TEXT("KMGC.SyntaxHighlight.Function");
					TextStyle = SyntaxTextStyle.FunctionTextStyle;
				}///
			break;}
			//
			if (const FString*F=DB->Extensions.Find(Keyword)) {
				if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
					Info.Name = TEXT("KMGC.SyntaxHighlight.Function");
					TextStyle = SyntaxTextStyle.FunctionTextStyle;
				}///
			break;}
		}///
		//
		for (const UMGC_ClassDB* DB : SyntaxTextStyle.ClassDB) {
			if (Keyword.Contains(TEXT("."))) {
				TArray<FString>SubTokens;
				Keyword.ParseIntoArray(SubTokens,TEXT("."));
				//
				for (const FString &SubToken : SubTokens) {
					if (const FString*F=DB->ScriptCore.Find(SubToken)) {
						if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
							Info.Name = TEXT("KMGC.SyntaxHighlight.Function");
							TextStyle = SyntaxTextStyle.FunctionTextStyle;
						}///
					break;}
					//
					if (const FString*F=DB->Extensions.Find(SubToken)) {
						if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
							Info.Name = TEXT("KMGC.SyntaxHighlight.Function");
							TextStyle = SyntaxTextStyle.FunctionTextStyle;
						}///
					break;}
				}///
			} else {
				if (const FString*F=DB->ScriptCore.Find(Keyword)) {
					if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
						Info.Name = TEXT("KMGC.SyntaxHighlight.Class");
						TextStyle = SyntaxTextStyle.ClassTextStyle;
					}///
				break;}
				//
				if (const FString*F=DB->Extensions.Find(Keyword)) {
					if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
						Info.Name = TEXT("KMGC.SyntaxHighlight.Class");
						TextStyle = SyntaxTextStyle.ClassTextStyle;
					}///
				break;}
			}///
		}///
		//
		for (const UMGC_ClassDB* DB : SyntaxTextStyle.ClassDB) {
			if (const FString*F=DB->ScriptTypes.Find(Keyword)) {
				if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
					Info.Name = TEXT("KMGC.SyntaxHighlight.Type");
					TextStyle = SyntaxTextStyle.TypeTextStyle;
				}///
			break;}
		}///
		//
		for (const UMGC_KeywordDB* DB : SyntaxTextStyle.KeywordDB) {
			if (const FString*F=DB->Macros.Find(Keyword)) {
				if (F->Equals(Keyword,ESearchCase::CaseSensitive)) {
					Info.Name = TEXT("KMGC.SyntaxHighlight.Macro");
					TextStyle = SyntaxTextStyle.MacroTextStyle;
				}///
			break;}
		}///
	}///
	//
	if (Token.Type==FSyntaxTokenizer::ETokenType::Literal||!LoopBack) {
		if (ParserState==EParserState::LookingForSingleLineComment) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Comment");
			TextStyle = SyntaxTextStyle.CommentTextStyle;
		} else if (ParserState==EParserState::LookingForMultiLineComment) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.Comment");
			TextStyle = SyntaxTextStyle.CommentTextStyle;
		} else if (ParserState==EParserState::LookingForString) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.String");
			TextStyle = SyntaxTextStyle.StringTextStyle;
		} else if (ParserState==EParserState::LookingForChar) {
			Info.Name = TEXT("KMGC.SyntaxHighlight.String");
			TextStyle = SyntaxTextStyle.StringTextStyle;
		}///
	}///
}

uint32 FKMGC_TextSyntaxHighlighter::CountCharacters(const FString &Source, const TCHAR &Char) const {
	TArray<TCHAR>Chars = Source.GetCharArray();
	uint32 C=0;
	//
	for (const TCHAR CH : Chars) {
		if (CH==Char) {C++;}
	}///
	//
	return C;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////