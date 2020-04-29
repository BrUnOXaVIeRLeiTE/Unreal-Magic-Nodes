//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeLogger.h"
#include "MagicNodeRuntime.h"
#include "KMGC_KismetTypes.h"
#include "MagicNodeKismet_Shared.h"

#include "Runtime/Engine/Public/EngineUtils.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Parser:: Internal Types

enum MAGICNODEKISMET_API EMGC_CompilerResult : uint32 {
	None					=	0		/// No Compilation result.
	, UnknownException		=	1		/// Compiler Unknown Exceptions.
	, InternalError			=	2		/// Compiler Internal Error.
	, ParsingFailure		=	3		/// Nothing to be parsed, probably a empty header or script.
	, InvalidClass			=	4		/// Script class not found, invalid parent or self-reference.
	, InvalidTemplate		=	5		/// Script templates not found, maybe directory permissions issue.
	, InvalidSyntax			=	6		/// Script can't be parsed, unexpected or invalid syntax was found.
	, InvalidOutput			=	7		/// Script source files not written, maybe file permissions issue.
	, Compiled				=	8		/// Script successfully parsed and CPP Class successfully generated.
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Parser:: Internal Class

class IKMGC_ScriptParser {
private:
	static FClassDefinition ClassPointer;
	//
	static FString THeader;
	static FString TScript;
	static FString Docs;
protected:
	void KMGC_InitParser() {
		if (IKMGC_ScriptParser::Message.Num()==0) {
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::None,TEXT("OK"));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::UnknownException,TEXT("'Unknown Exception' while parsing MGC Script."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::InternalError,TEXT("'Internal Compiler Error' while parsing MGC Script."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::ParsingFailure,TEXT("'Parsing Failure' while parsing MGC Script; This is usually caused by empty scripts or headers."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::InvalidClass,TEXT("'Invalid Class' while parsing MGC Script; This is usually caused by an unknown or self-referenced parent class."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::InvalidTemplate,TEXT("'Invalid Template' while parsing MGC Script; This is usually caused by OS limited permissions when reading Content/Template/ directory."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::InvalidSyntax,TEXT("'Invalid Syntax' while parsing MGC Script; This is usually caused by invalid or missing 'Execute' function signature."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::InvalidOutput,TEXT("'Invalid Output' while parsing MGC Script; This is usually caused by OS limited permissions when writing to Source/Project/MagicNodes/ directory."));
			IKMGC_ScriptParser::Message.Add((uint32)EMGC_CompilerResult::Compiled,TEXT("MGC 'Compiled', Script successfully parsed and CPP Class successfully generated."));
		}///
	}///
	//
	MAGICNODEKISMET_API const EMGC_CompilerResult CompileScriptClass(const FString &ScriptName, const FString &Header, const FString &Script, const FString &Types, const FString &ParentClass, const TArray<FString>&Includes, const TArray<FString>&Macros);
protected:
	MAGICNODEKISMET_API static const bool BuildAutoComplete(const FClassDefinition &Definition, TArray<FString>&Results);
	MAGICNODEKISMET_API static const bool ParseHeaderIsland(const FString &Class, const FString &Island, FClassDefinition &ClassInfo, EAccessLevel &Level);
public:
	MAGICNODEKISMET_API static const FFunctionDefinition NOFunctionInfo;
	MAGICNODEKISMET_API static const FPropertyDefinition NOPropertyInfo;
	MAGICNODEKISMET_API static const FKeywordDefinition NOKeywordInfo;
	MAGICNODEKISMET_API static const FClassRedirector NORedirector;
	MAGICNODEKISMET_API static const FClassDefinition NOClassInfo;
	//
	MAGICNODEKISMET_API static TMap<uint32,FString>Message;
public:
	MAGICNODEKISMET_API static FClassDefinition &GetClassReference(const FString &Keyword);
	MAGICNODEKISMET_API static const FClassDefinition &GetClassInfo(const FString &Keyword);
	MAGICNODEKISMET_API static const FClassRedirector &GetClassRedirector(const FString &Keyword);
	MAGICNODEKISMET_API static const FClassDefinition &GetClassPointerInfo(const FString &Keyword);
	//
	MAGICNODEKISMET_API static const FKeywordDefinition &GetKeywordInfo(const FString &Keyword);
	MAGICNODEKISMET_API static const FPropertyDefinition &GetPropertyInfo(const FString &Keyword);
	MAGICNODEKISMET_API static const FPropertyDefinition &GetPropertyInfo(const UClass* FromClass, const FString &Keyword);
	MAGICNODEKISMET_API static const FPropertyDefinition &GetPropertyInfo(const FString &FromClass, const FString &Keyword);
	MAGICNODEKISMET_API static const FPropertyDefinition &GetPropertyInfo(const UScriptStruct* FromStruct, const FString &Keyword);
	//
	MAGICNODEKISMET_API static const FFunctionDefinition &GetFunctionInfo(const FString &Keyword);
	MAGICNODEKISMET_API static const FFunctionDefinition &GetFunctionInfo(const UClass* FromClass, const FString &Keyword);
	MAGICNODEKISMET_API static const FFunctionDefinition &GetFunctionInfo(const FString &FromClass, const FString &Keyword);
	//
	MAGICNODEKISMET_API static const FSlateColor GetTypeColor(const FString &Notation);
	MAGICNODEKISMET_API static const FSlateBrush* GetTypeIcon(const FString &Notation);
public:
	MAGICNODEKISMET_API static void AutoComplete(const FString &Owner, const FString &Keyword, TArray<FString>&Results);
	MAGICNODEKISMET_API static void AutoSuggest(const TArray<FString>&Lines, const FString &Keyword, TArray<FString>&Results);
	//
	MAGICNODEKISMET_API static const FString &GetClassDocumentation(const FString &Keyword);
	MAGICNODEKISMET_API static const bool ParseClassFromHeader(const FString &Header, const FString &Class);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////