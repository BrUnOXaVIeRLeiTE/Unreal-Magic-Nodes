//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_ScriptParser.h"
#include "KMGC_KismetTypes.h"

#include "Runtime/Core/Public/Misc/App.h"
#include "Runtime/Core/Public/Misc/Paths.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"

#include "Interfaces/IPluginManager.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const FFunctionDefinition IKMGC_ScriptParser::NOFunctionInfo = FFunctionDefinition();
const FPropertyDefinition IKMGC_ScriptParser::NOPropertyInfo = FPropertyDefinition();
const FKeywordDefinition IKMGC_ScriptParser::NOKeywordInfo = FKeywordDefinition();
const FClassRedirector IKMGC_ScriptParser::NORedirector = FClassRedirector();
const FClassDefinition IKMGC_ScriptParser::NOClassInfo = FClassDefinition();

FClassDefinition IKMGC_ScriptParser::ClassPointer = FClassDefinition();
TMap<uint32,FString>IKMGC_ScriptParser::Message = TMap<uint32,FString>();

FString IKMGC_ScriptParser::THeader = TEXT("");
FString IKMGC_ScriptParser::TScript = TEXT("");
FString IKMGC_ScriptParser::Docs = TEXT("");


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const EMGC_CompilerResult IKMGC_ScriptParser::CompileScriptClass(const FString &ScriptName, const FString &Header, const FString &Script, const FString &Types, const FString &ParentClass, const TArray<FString>&Includes, const TArray<FString>&Macros) {
	if (Header.IsEmpty()||Script.IsEmpty()) {return EMGC_CompilerResult::ParsingFailure;}
	EMGC_CompilerResult Result = EMGC_CompilerResult::None;
	//
	//
	/// :: SETUP ::
	///
	static FString Sources = FPaths::ConvertRelativePathToFull(FPaths::GameSourceDir()+FString(TEXT("/"))+FString(FApp::GetProjectName()));
	static FString Content = FPaths::ConvertRelativePathToFull(IPluginManager::Get().FindPlugin(TEXT("MagicNode"))->GetContentDir());
	//
	const bool TS_Loaded = FFileHelper::LoadFileToString(IKMGC_ScriptParser::TScript,*(Content+TEXT("/Template/__CPP.txt")));
	const bool TH_Loaded = FFileHelper::LoadFileToString(IKMGC_ScriptParser::THeader,*(Content+TEXT("/Template/__H.txt")));
	if (!(TH_Loaded && TS_Loaded)) {Result=EMGC_CompilerResult::InvalidTemplate; return Result;}
	//
	//
	/// :: PARSE MAIN IDENTIFIERS ::
	///
	FString Name = ScriptName;
	Name.RemoveFromEnd(TEXT("_C"));
	Name.RemoveFromStart(TEXT("Default__"));
	//
	if (Name==ParentClass||(Name+TEXT("__CPP"))==ParentClass) {
		Result = EMGC_CompilerResult::InvalidClass;
		LOG_MGC(EMGCSeverity::Error,FString::Printf(TEXT("'%s' just tried to inherit itself."),*Name));
	return Result;}
	//
	FString ProjectName = FApp::GetProjectName();
	ProjectName.ToUpperInline();
	//
	//
	/// :: CLEANUP ::
	///
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("\n\n\n\n\n"),TEXT(""));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{ProjectName}"),*ProjectName);
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{ParentClass}"),*ParentClass);
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{ScriptClass}"),*Name);
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{ScriptName}"),*Name);
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{Header}"),*Header);
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{Types}"),*Types);
	//
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("\n\n\n\n\n"),TEXT(""));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("{ScriptClass}"),*Name);
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("{ScriptName}"),*Name);
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("{Script}"),*Script);
	//
	//
	/// :: PARSE INCLUDES ::
	///
	FString Include;
	for (FString In : Includes) {
		if (!In.Contains(TEXT(".h"))) {continue;}
		//
		if (!In.Contains(TEXT("#include"))) {
			FString InFormat = FString::Printf(TEXT("#include \"%s\""),*In);
			Include.Append(InFormat); Include.AppendChar(TEXT('\n'));
		} else {
			Include.Append(In);
			Include.AppendChar(TEXT('\n'));
		}///
	}///
	Include.RemoveFromEnd(TEXT("\n"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{Includes}"),*Include);
	//
	//
	/// :: PARSE MACROS ::
	///
	FString Macroe;
	for (FString In : Macros) {
		Macroe.Append(In);
		Macroe.AppendChar(TEXT('\n'));
	}///
	Macroe.RemoveFromEnd(TEXT("\n"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("{Macros}"),*Macroe);
	//
	//
	/// :: PARSE SYNTAX (Header Check) ::
	///
	if (!IKMGC_ScriptParser::THeader.Contains(TEXT("Execute"),ESearchCase::CaseSensitive)) {Result=EMGC_CompilerResult::InvalidSyntax;}
	if (!IKMGC_ScriptParser::TScript.Contains(TEXT("Execute"),ESearchCase::CaseSensitive)) {Result=EMGC_CompilerResult::InvalidSyntax;}
	//
	FString HeaderCheck = IKMGC_ScriptParser::THeader;
	HeaderCheck.ReplaceInline(TEXT("\"false\""),TEXT("false"));
	HeaderCheck.ReplaceInline(TEXT("\"true\""),TEXT("true"));
	HeaderCheck.ReplaceInline(TEXT("\n"),TEXT(""));
	HeaderCheck.ReplaceInline(TEXT(" "),TEXT(""));
	//
	if (!HeaderCheck.Contains(TEXT("IMGC"),ESearchCase::CaseSensitive)) {
	if (!HeaderCheck.Contains(TEXT("staticvoidExecute("),ESearchCase::CaseSensitive)) {
		LOG_MGC(EMGCSeverity::Error,TEXT("'Execute()' function definition not found. Declaration must be 'static' and must be 'void'."));
		Result=EMGC_CompilerResult::InvalidSyntax;
	} if (!HeaderCheck.Contains(TEXT("BlueprintInternalUseOnly=true"),ESearchCase::CaseSensitive)) {
		LOG_MGC(EMGCSeverity::Error,TEXT("'BlueprintInternalUseOnly' property must exist and set to true on 'Execute' function."));
		Result=EMGC_CompilerResult::InvalidSyntax;
	} if (!HeaderCheck.Contains(TEXT("WorldContext="),ESearchCase::CaseSensitive)) {
		LOG_MGC(EMGCSeverity::Error,TEXT("'WorldContext' property must exist on 'Execute' function."));
		Result=EMGC_CompilerResult::InvalidSyntax;
	} if (!HeaderCheck.Contains(TEXT("BlueprintCallable"),ESearchCase::CaseSensitive)) {
		LOG_MGC(EMGCSeverity::Error,TEXT("'BlueprintCallable' property must exist on 'Execute' function."));
		Result=EMGC_CompilerResult::InvalidSyntax;
	}}///
	//
	//
	/// :: PARSE SYNTAX (Script Types to C++ Types) ::
	///
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("IMGC ( )"),TEXT("UFUNCTION (BlueprintCallable,Category=\"Magic Node\",meta=(WorldContext=\"Context\",BlueprintInternalUseOnly=\"true\",ToolTip=\"FSelf\"))\n"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("IMGC ()"),TEXT("UFUNCTION (BlueprintCallable,Category=\"Magic Node\",meta=(WorldContext=\"Context\",BlueprintInternalUseOnly=\"true\",ToolTip=\"FSelf\"))\n"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("IMGC( )"),TEXT("UFUNCTION (BlueprintCallable,Category=\"Magic Node\",meta=(WorldContext=\"Context\",BlueprintInternalUseOnly=\"true\",ToolTip=\"FSelf\"))\n"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("IMGC()"),TEXT("UFUNCTION (BlueprintCallable,Category=\"Magic Node\",meta=(WorldContext=\"Context\",BlueprintInternalUseOnly=\"true\",ToolTip=\"FSelf\"))\n"));
	//
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FSelf"),*(FString(TEXT("U"))+Name+FString(TEXT("__CPP"))));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FSelf"),*(FString(TEXT("U"))+Name+FString(TEXT("__CPP"))));
	//
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FBool"),TEXT("bool"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FFloat"),TEXT("float"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FDouble"),TEXT("double"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FUByte"),TEXT("uint8"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FUShort"),TEXT("uint16"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FUInt"),TEXT("uint32"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FULong"),TEXT("uint64"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FByte"),TEXT("int8"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FShort"),TEXT("int16"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FInt"),TEXT("int32"));
	IKMGC_ScriptParser::THeader.ReplaceInline(TEXT("FLong"),TEXT("int64"));
	//
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FBool"),TEXT("bool"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FFloat"),TEXT("float"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FDouble"),TEXT("double"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FUByte"),TEXT("uint8"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FUShort"),TEXT("uint16"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FUInt"),TEXT("uint32"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FULong"),TEXT("uint64"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FByte"),TEXT("int8"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FShort"),TEXT("int16"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FInt"),TEXT("int32"));
	IKMGC_ScriptParser::TScript.ReplaceInline(TEXT("FLong"),TEXT("int64"));
	//
	//
	/// :: FINALIZE ::
	///
	if (Result != EMGC_CompilerResult::InvalidSyntax) {
		const bool TH_Saved = FFileHelper::SaveStringToFile(IKMGC_ScriptParser::THeader,*(Sources+FString(TEXT("/MagicNodes/"))+Name+TEXT("__CPP.h")));
		const bool TS_Saved = FFileHelper::SaveStringToFile(IKMGC_ScriptParser::TScript,*(Sources+FString(TEXT("/MagicNodes/"))+Name+TEXT("__CPP.cpp")));
		//
		if (TH_Saved && TS_Saved) {Result=EMGC_CompilerResult::Compiled;} else {Result=EMGC_CompilerResult::InvalidOutput;}
	} return Result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const FKeywordDefinition & IKMGC_ScriptParser::GetKeywordInfo(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			if (DB.Get()->KeywordDefinitions.Contains(Keyword)) {return DB.Get()->KeywordDefinitions.FindChecked(Keyword);}
			if (DB.Get()->MacroDefinitions.Contains(Keyword)) {return DB.Get()->MacroDefinitions.FindChecked(Keyword);}
			if (DB.Get()->TypeDefinitions.Contains(Keyword)) {return DB.Get()->TypeDefinitions.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::NOKeywordInfo;
}

FClassDefinition & IKMGC_ScriptParser::GetClassReference(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			if (DB.Get()->ClassDefinitions.Contains(Keyword)) {return DB.Get()->ClassDefinitions.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::ClassPointer;
}

const FClassDefinition & IKMGC_ScriptParser::GetClassInfo(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			if (DB.Get()->ClassDefinitions.Contains(Keyword)) {return DB.Get()->ClassDefinitions.FindChecked(Keyword);}
		}///
	}///
	//
	//
	UMGC_SemanticDB* DB = _Settings->SemanticDB.Array()[0].Get();
	if (DB==nullptr) {IKMGC_ScriptParser::ClassPointer;}
	//
	FString NPK = Keyword;
	if (NPK.RemoveFromStart(TEXT("A"),ESearchCase::CaseSensitive)) {
		if (UClass*Class=FindObject<UClass>(ANY_PACKAGE,*NPK,true)) {
			DB->RegisterClassReflection(Class,Class->GetPrefixCPP());
			return DB->ClassDefinitions.FindChecked(Class->GetPrefixCPP()+NPK);
		}///
	} else if (NPK.RemoveFromStart(TEXT("U"),ESearchCase::CaseSensitive)) {
		if (UClass*Class=FindObject<UClass>(ANY_PACKAGE,*NPK,true)) {
			DB->RegisterClassReflection(Class,Class->GetPrefixCPP());
			return DB->ClassDefinitions.FindChecked(Class->GetPrefixCPP()+NPK);
		}///
	} else if (NPK.RemoveFromStart(TEXT("F"),ESearchCase::CaseSensitive)) {
		if (UScriptStruct*Struct=FindObject<UScriptStruct>(ANY_PACKAGE,*NPK,true)) {
			DB->RegisterStructReflection(Struct);
			return DB->ClassDefinitions.FindChecked(Struct->GetPrefixCPP()+NPK);
		}///
	} else if (UEnum*Enum=FindObject<UEnum>(ANY_PACKAGE,*Keyword,true)) {
		DB->RegisterEnumReflection(Enum);
		return DB->ClassDefinitions.FindChecked(Keyword);
	}///
	//
	return IKMGC_ScriptParser::NOClassInfo;
}

const FClassDefinition & IKMGC_ScriptParser::GetClassPointerInfo(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassRedirectors.Contains(Keyword)) {
			const FClassRedirector &Instance = DB.Get()->ClassRedirectors.FindChecked(Keyword);
			if (DB.Get()->ClassDefinitions.Contains(Instance.ObjectClass)) {
				IKMGC_ScriptParser::ClassPointer = DB.Get()->ClassDefinitions.FindChecked(Instance.ObjectClass);
				if (!Instance.Hint.IsEmpty()) {IKMGC_ScriptParser::ClassPointer.Hint=Instance.Hint;}
				//
				return IKMGC_ScriptParser::ClassPointer;
			} else if (DB.Get()->ClassDefinitions.Contains(Instance.OwnerClass)) {
				const FClassDefinition OwnerClass = DB.Get()->ClassDefinitions.FindChecked(Instance.OwnerClass);
				if (OwnerClass.Functions.Contains(Instance.ObjectName)) {
					const FFunctionDefinition &FunInfo = IKMGC_ScriptParser::GetFunctionInfo(Instance.ObjectName);
					FString Subject = FunInfo.ReturnType; FString Container;
					//
					FunInfo.ReturnType.Split(TEXT("< "),&Container,&Subject);
					Subject.Split(TEXT(" "),&Subject,nullptr);
					//
					if (!Container.IsEmpty()) {
						return IKMGC_ScriptParser::GetClassInfo(Container);
					} else {
						return IKMGC_ScriptParser::GetClassInfo(Subject);
					}///
				} else if (OwnerClass.Variables.Contains(Instance.ObjectName)) {
					const FPropertyDefinition &PropInfo = IKMGC_ScriptParser::GetPropertyInfo(Instance.ObjectName);
					FString Subject = PropInfo.ReturnType; FString Container;
					//
					PropInfo.ReturnType.Split(TEXT("< "),&Container,&Subject);
					Subject.Split(TEXT(" "),&Subject,nullptr);
					//
					if (!Container.IsEmpty()) {
						return IKMGC_ScriptParser::GetClassInfo(Container);
					} else {
						return IKMGC_ScriptParser::GetClassInfo(Subject);
					}///
				}///
			}///
		} else {return IKMGC_ScriptParser::GetClassInfo(Keyword);}
	}///
	//
	return IKMGC_ScriptParser::NOClassInfo;
}

const FClassRedirector & IKMGC_ScriptParser::GetClassRedirector(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassRedirectors.Contains(Keyword)) {
			return DB.Get()->ClassRedirectors.FindChecked(Keyword);
		}///
	}///
	//
	return IKMGC_ScriptParser::NORedirector;
}

const FPropertyDefinition & IKMGC_ScriptParser::GetPropertyInfo(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			TArray<FClassDefinition>Classes;
			DB.Get()->ClassDefinitions.GenerateValueArray(Classes);
			//
			for (const auto &ClassInfo : Classes) {
				if (ClassInfo.Variables.Contains(Keyword)) {return ClassInfo.Variables.FindChecked(Keyword);}
			}///
		}///
	}///
	//
	return IKMGC_ScriptParser::NOPropertyInfo;
}

const FPropertyDefinition & IKMGC_ScriptParser::GetPropertyInfo(const UClass* FromClass, const FString &Keyword) {
	if (FromClass==nullptr) {return IKMGC_ScriptParser::NOPropertyInfo;}
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassDefinitions.Contains(FString(TEXT("U"))+FromClass->GetName())) {
			const FClassDefinition &ClassInfo = DB.Get()->ClassDefinitions.FindChecked(FString(TEXT("U"))+FromClass->GetName());
			if (ClassInfo.Variables.Contains(Keyword)) {return ClassInfo.Variables.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::NOPropertyInfo;
}

const FPropertyDefinition & IKMGC_ScriptParser::GetPropertyInfo(const UScriptStruct* FromStruct, const FString &Keyword) {
	if (FromStruct==nullptr) {return IKMGC_ScriptParser::NOPropertyInfo;}
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassDefinitions.Contains(FString(TEXT("F"))+FromStruct->GetName())) {
			const FClassDefinition &ClassInfo = DB.Get()->ClassDefinitions.FindChecked(FString(TEXT("F"))+FromStruct->GetName());
			if (ClassInfo.Variables.Contains(Keyword)) {return ClassInfo.Variables.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::NOPropertyInfo;
}

const FPropertyDefinition & IKMGC_ScriptParser::GetPropertyInfo(const FString &FromClass, const FString &Keyword) {
	if (FromClass.TrimStartAndEnd().IsEmpty()) {return IKMGC_ScriptParser::NOPropertyInfo;}
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassDefinitions.Contains(FromClass)) {
			const FClassDefinition &ClassInfo = DB.Get()->ClassDefinitions.FindChecked(FromClass);
			if (ClassInfo.Variables.Contains(Keyword)) {return ClassInfo.Variables.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::NOPropertyInfo;
}

const FFunctionDefinition & IKMGC_ScriptParser::GetFunctionInfo(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			TArray<FClassDefinition>Classes;
			DB.Get()->ClassDefinitions.GenerateValueArray(Classes);
			//
			for (const auto &ClassInfo : Classes) {
				if (ClassInfo.Functions.Contains(Keyword)) {return ClassInfo.Functions.FindChecked(Keyword);}
			}///
		}///
	}///
	//
	return IKMGC_ScriptParser::NOFunctionInfo;
}

const FFunctionDefinition & IKMGC_ScriptParser::GetFunctionInfo(const UClass* FromClass, const FString &Keyword) {
	if (FromClass==nullptr) {return IKMGC_ScriptParser::NOFunctionInfo;}
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassDefinitions.Contains(FString(TEXT("U"))+FromClass->GetName())) {
			const FClassDefinition &ClassInfo = DB.Get()->ClassDefinitions.FindChecked(FString(TEXT("U"))+FromClass->GetName());
			if (ClassInfo.Functions.Contains(Keyword)) {return ClassInfo.Functions.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::NOFunctionInfo;
}

const FFunctionDefinition & IKMGC_ScriptParser::GetFunctionInfo(const FString &FromClass, const FString &Keyword) {
	if (FromClass.TrimStartAndEnd().IsEmpty()) {return IKMGC_ScriptParser::NOFunctionInfo;}
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()&&DB.Get()->ClassDefinitions.Contains(FromClass)) {
			const FClassDefinition &ClassInfo = DB.Get()->ClassDefinitions.FindChecked(FromClass);
			if (ClassInfo.Functions.Contains(Keyword)) {return ClassInfo.Functions.FindChecked(Keyword);}
		}///
	}///
	//
	return IKMGC_ScriptParser::NOFunctionInfo;
}

const FString & IKMGC_ScriptParser::GetClassDocumentation(const FString &Keyword) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	for (auto DB : _Settings->SemanticDB.Array()) {
		if (DB.IsValid()) {
			if (DB.Get()->Documentation.Contains(Keyword)) {return DB.Get()->Documentation.FindChecked(Keyword);}
		}///
	}///
	//
	return Docs;
}

const FSlateColor IKMGC_ScriptParser::GetTypeColor(const FString &Notation) {
	FSlateColor Color = FSlateColor(FLinearColor::White);
	//
	if (Notation.Contains(TEXT("STT|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0,0,0.8f)); return Color;}
	if (Notation.Contains(TEXT("BOL|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.7f,0,0)); return Color;}
	if (Notation.Contains(TEXT("FLO|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0,0.8f,0)); return Color;}
	if (Notation.Contains(TEXT("ENU|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0,0.8f,0.8f)); return Color;}
	if (Notation.Contains(TEXT("BYT|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0,0.4f,0.25f)); return Color;}
	if (Notation.Contains(TEXT("STR|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.85f,0,0.6f)); return Color;}
	if (Notation.Contains(TEXT("INT|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0,0.95f,0.5f)); return Color;}
	if (Notation.Contains(TEXT("NAM|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.55f,0.35f,0.8f)); return Color;}
	if (Notation.Contains(TEXT("TEX|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.85f,0.4f,0.55f)); return Color;}
	if (Notation.Contains(TEXT("ARR|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.55f,0.45f,0.85f)); return Color;}
	if (Notation.Contains(TEXT("SET|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.45f,0.85f,0.55f)); return Color;}
	if (Notation.Contains(TEXT("MAP|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.85f,0.55f,0.45f)); return Color;}
	if (Notation.Contains(TEXT("OBJ|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.45f,0.45f,0.85f)); return Color;}
	if (Notation.Contains(TEXT("CLS|"),ESearchCase::CaseSensitive)) {Color=FSlateColor(FLinearColor(0.85f,0.35f,0.65f)); return Color;}
	//
	return Color;
}

const FSlateBrush* IKMGC_ScriptParser::GetTypeIcon(const FString &Notation) {
	const FSlateBrush* Brush = FEditorStyle::GetBrush(TEXT("Kismet.VariableList.TypeIcon"));
	//
	if (Notation.Contains(TEXT("OBJ|"),ESearchCase::CaseSensitive)) {Brush=FEditorStyle::GetBrush(TEXT("CodeView.ClassIcon"));}
	if (Notation.Contains(TEXT("SET|"),ESearchCase::CaseSensitive)) {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.SetTypeIcon"));}
	if (Notation.Contains(TEXT("ARR|"),ESearchCase::CaseSensitive)) {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.ArrayTypeIcon"));}
	if (Notation.Contains(TEXT("MAP|"),ESearchCase::CaseSensitive)) {Brush=FEditorStyle::GetBrush(TEXT("Kismet.VariableList.MapValueTypeIcon"));}
	//
	return Brush;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IKMGC_ScriptParser::AutoComplete(const FString &Owner, const FString &Keyword, TArray<FString>&Results) {
	const FClassDefinition &ClassInfo = IKMGC_ScriptParser::GetClassInfo(Keyword);
	//
	if (!IKMGC_ScriptParser::BuildAutoComplete(ClassInfo,Results)) {
		const FClassDefinition &ClassPtr = IKMGC_ScriptParser::GetClassPointerInfo(Keyword);
		//
		if (!IKMGC_ScriptParser::BuildAutoComplete(ClassPtr,Results)) {
			const FClassDefinition &OwnerPointer = IKMGC_ScriptParser::GetClassPointerInfo(Owner);
			if (Keyword.IsEmpty()&&(IKMGC_ScriptParser::BuildAutoComplete(OwnerPointer,Results))) {return;}
			//
			const FFunctionDefinition &FunInfo = IKMGC_ScriptParser::GetFunctionInfo(Owner,Keyword);
			if (FunInfo != IKMGC_ScriptParser::NOFunctionInfo) {
				FString Subject = FunInfo.ReturnType;
				FString Container;
				//
				FunInfo.ReturnType.Split(TEXT("< "),&Container,&Subject);
				Subject.Split(TEXT(" "),&Subject,nullptr);
				//
				if (!Container.IsEmpty()) {
					const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Container);
					IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
				} else {
					const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Subject);
					IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
				}///
			} else { 
				const FClassRedirector &Redirect = IKMGC_ScriptParser::GetClassRedirector(Owner);
				const FFunctionDefinition &Fun2Info = IKMGC_ScriptParser::GetFunctionInfo(Redirect.ObjectClass,Keyword);
				if (Fun2Info != IKMGC_ScriptParser::NOFunctionInfo) {
					FString Subject = Fun2Info.ReturnType;
					FString Container;
					//
					Fun2Info.ReturnType.Split(TEXT("< "),&Container,&Subject);
					Subject.Split(TEXT(" "),&Subject,nullptr);
					//
					if (!Container.IsEmpty()) {
						const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Container);
						IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
					} else {
						const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Subject);
						IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
					}///
				} else {
					const FPropertyDefinition &PropInfo = IKMGC_ScriptParser::GetPropertyInfo(Owner,Keyword);
					if (PropInfo != IKMGC_ScriptParser::NOPropertyInfo) {
						FString Subject = PropInfo.ReturnType;
						FString Container;
						//
						PropInfo.ReturnType.Split(TEXT("< "),&Container,&Subject);
						Subject.Split(TEXT(" "),&Subject,nullptr);
						//
						if (!Container.IsEmpty()) {
							const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Container);
							IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
						} else {
							const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Subject);
							IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
						}///
					} else {
						const FPropertyDefinition &Prop2Info = IKMGC_ScriptParser::GetPropertyInfo(Redirect.ObjectClass,Keyword);
						if (Prop2Info != IKMGC_ScriptParser::NOPropertyInfo) {
							FString Subject = Prop2Info.ReturnType;
							FString Container;
							//
							Prop2Info.ReturnType.Split(TEXT("< "),&Container,&Subject);
							Subject.Split(TEXT(" "),&Subject,nullptr);
							//
							if (!Container.IsEmpty()) {
								const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Container);
								IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
							} else {
								const FClassDefinition &ReturnInfo = IKMGC_ScriptParser::GetClassPointerInfo(Subject);
								IKMGC_ScriptParser::BuildAutoComplete(ReturnInfo,Results);
							}///
						}///
					}///
				}///
			}///
		}///
	}///
}

const bool IKMGC_ScriptParser::BuildAutoComplete(const FClassDefinition &Definition, TArray<FString>&Results) {
	if (Definition != IKMGC_ScriptParser::NOClassInfo) {
		TArray<FString>Vars; Definition.Variables.GetKeys(Vars);
		TArray<FString>Funs; Definition.Functions.GetKeys(Funs);
		//
		for (const FString &Item : Vars) {
			const FPropertyDefinition &DEF = Definition.Variables.FindChecked(Item);
			FString Result = FString(TEXT("VAR|"));
			//
			switch (DEF.Access) {
				case EAccessLevel::Public: Result += FString(TEXT("PB|")); break;
				case EAccessLevel::Static: Result += FString(TEXT("ST|")); break;
				case EAccessLevel::Private: Result += FString(TEXT("PV|")); break;
				case EAccessLevel::Protected: Result += FString(TEXT("PT|")); break;
			default: break;}
			//
			switch (DEF.TypeOf) {
				case EType::Int: Result += FString(TEXT("INT|")); break;
				case EType::Byte: Result += FString(TEXT("BYT|")); break;
				case EType::Bool: Result += FString(TEXT("BOL|")); break;
				case EType::Name: Result += FString(TEXT("NAM|")); break;
				case EType::Text: Result += FString(TEXT("TEX|")); break;
				case EType::Float: Result += FString(TEXT("FLO|")); break;
				case EType::String: Result += FString(TEXT("STR|")); break;
				//
				case EType::Enum: Result += FString(TEXT("ENU|")); break;
				case EType::Class: Result += FString(TEXT("CLS|")); break;
				case EType::Struct: Result += FString(TEXT("STT|")); break;
				case EType::Object: Result += FString(TEXT("OBJ|")); break;
			default: break;}
			//
			switch (DEF.StackOf) {
				case EStack::Set: Result += FString(TEXT("SET|")); break;
				case EStack::Map: Result += FString(TEXT("MAP|")); break;
				case EStack::Array: Result += FString(TEXT("ARR|")); break;
			default: break;}
			//
			Result += Item;
			Results.Add(Result);
		}///
		//
		for (const FString &Item : Funs) {
			const FFunctionDefinition &DEF = Definition.Functions.FindChecked(Item);
			FString Result = FString(TEXT("FUN|"));
			//
			switch (DEF.Access) {
				case EAccessLevel::Public: Result += FString(TEXT("PB|")); break;
				case EAccessLevel::Static: Result += FString(TEXT("ST|")); break;
				case EAccessLevel::Private: Result += FString(TEXT("PV|")); break;
				case EAccessLevel::Protected: Result += FString(TEXT("PT|")); break;
			default: break;}
			//
			Result += Item;
			Results.Add(Result);
		}///
	} else {return false;}
	//
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void IKMGC_ScriptParser::AutoSuggest(const TArray<FString>&Lines, const FString &Keyword, TArray<FString>&Results) {
	const auto &MGC_Settings = GetDefault<UKMGC_Settings>();
	//
	static const TCHAR* Whitespace[] = {
		TEXT("\t "),TEXT("\t"),TEXT(" "),
		TEXT("("),TEXT(")"),TEXT("&"),
		TEXT("["),TEXT("]"),TEXT("*"),
		TEXT("{"),TEXT("}"),TEXT("?"),
		TEXT("<"),TEXT(">"),TEXT(":"),
		TEXT("."),TEXT(","),TEXT(";"),
		TEXT("-"),TEXT("+"),TEXT("="),
		TEXT("!"),TEXT("^"),TEXT("#"),
		TEXT("%"),TEXT("/"),TEXT("|"),
		TEXT("\'"),TEXT("\""),TEXT("\\")
	};//
	//
	for (const FString &Item : Lines) {
		if (!Item.Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
		TArray<FString>Line;
		int32 Nums = UE_ARRAY_COUNT(Whitespace);	
		Item.ParseIntoArray(Line,Whitespace,Nums,true);
		//
		for (const FString &Key : Line) {
			if (Results.Num()>=MAX_SUGGESTIONS) {return;}
			if (!Key.Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
			//
			FString Clean;
			for (const TCHAR &CH : Key) {
				if ((TChar<WIDECHAR>::IsAlpha(CH)||TChar<WIDECHAR>::IsDigit(CH)||CH==TEXT('_'))) {Clean.AppendChar(CH);}
			}///
			//
			if (Keyword.Contains(Clean)||Clean.Len()<Keyword.Len())
			{continue;} else {Results.AddUnique(Clean);}
		}///
	}///
	//
	for (auto DB : MGC_Settings->KeywordDB.Array()) {
		auto DTB = DB.Get(); if (DTB) {
			for (auto IT=DTB->ScriptCore.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
			//
			for (auto IT=DTB->Macros.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
			//
			for (auto IT=DTB->Processors.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
			//
			for (auto IT=DTB->Extensions.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
		}///
	}///
	//
	for (auto DB : MGC_Settings->ClassDB.Array()) {
		auto DTB = DB.Get(); if (DTB) {
			for (auto IT=DTB->ScriptCore.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
			//
			for (auto IT=DTB->ScriptTypes.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
			//
			for (auto IT=DTB->Extensions.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
		}///
	}///
	//
	for (auto DB : MGC_Settings->FunctionDB.Array()) {
		auto DTB = DB.Get(); if (DTB) {
			for (auto IT=DTB->ScriptCore.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
			//
			for (auto IT=DTB->Extensions.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains((*IT),ESearchCase::CaseSensitive)||IT->Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(*IT);}
			}///
		}///
	}///
	//
	for (auto DB : MGC_Settings->SemanticDB.Array()) {
		auto DTB = DB.Get(); if (DTB) {
			for (auto IT=DTB->ClassDefinitions.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Key.Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains(IT->Key,ESearchCase::CaseSensitive)||IT->Key.Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(IT->Key);}
			}///
			//
			for (auto IT=DTB->ClassRedirectors.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Key.Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains(IT->Key,ESearchCase::CaseSensitive)||IT->Key.Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(IT->Key);}
			}///
			//
			for (auto IT=DTB->MacroDefinitions.CreateConstIterator(); IT; ++IT) {
				if (Results.Num()>=MAX_SUGGESTIONS) {return;}
				//
				if (!IT->Key.Contains(Keyword,ESearchCase::CaseSensitive)) {continue;}
				if (Keyword.Contains(IT->Key,ESearchCase::CaseSensitive)||IT->Key.Len()<Keyword.Len())
				{continue;} else {Results.AddUnique(IT->Key);}
			}///
		}///
	}///
	//
	Results.Sort();
	//
	if (Results.Num()==0) {
		//@ToDo: Search C++/H files...
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const bool IKMGC_ScriptParser::ParseClassFromHeader(const FString &Header, const FString &Class) {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	if (Header.TrimStartAndEnd().IsEmpty()) {return false;}
	if (Class.TrimStartAndEnd().IsEmpty()) {return false;}
	//
	if (_Settings->SemanticDB.Num()==0) {return false;}
	if (_Settings->FunctionDB.Num()==0) {return false;}
	//
	UMGC_SemanticDB* SemanticDB = _Settings->SemanticDB.Array()[0].Get();
	UMGC_FunctionDB* FunctionDB = _Settings->FunctionDB.Array()[0].Get();
	//
	if (SemanticDB==nullptr) {return false;}
	if (FunctionDB==nullptr) {return false;}
	//
	//
	FString Parse;
	FString Raw = FString(Header);
	{///
		int32 I=0;
		bool Search=false;
		//
		TCHAR IT=TEXT('\n');
		TCHAR ITL=TEXT('\n');
		//
		while (I<Raw.Len()) {
			ITL=IT; IT=Raw[I];
			//
			if ((!Search)&&(ITL==TEXT('/')&&(IT==TEXT('*')))) {Search=true; Parse.AppendChar(TEXT('|')); Parse.AppendChar(TEXT('~')); Parse.AppendChar(TEXT('|')); Parse.AppendChar(ITL);} else
			if ((Search)&&(ITL==TEXT('*')&&(IT==TEXT('/')))) {Search=false; Parse.AppendChar(IT);}
			//
			if (Search) {Parse.AppendChar(IT);}
		++I;}
		//
		TArray<FString>Comments;
		Parse.ParseIntoArray(Comments,TEXT("|~|"));
		for (const FString &C : Comments) {
			Raw.ReplaceInline(*C,TEXT(""));
		}///
	}///
	Parse.Empty();
	{///
		int32 I=0;
		bool Search=false;
		//
		TCHAR IT=TEXT('\n');
		TCHAR ITL=TEXT('\n');
		//
		while (I<Raw.Len()) {
			ITL=IT; IT=Raw[I];
			//
			if ((!Search)&&(ITL==TEXT('/')&&(IT==TEXT('/')))) {Search=true; Parse.AppendChar(TEXT('|')); Parse.AppendChar(TEXT('~')); Parse.AppendChar(TEXT('|')); Parse.AppendChar(ITL);} else
			if ((Search)&&(IT==TEXT('\n'))) {Search=false; Parse.AppendChar(IT);}
			//
			if (Search) {Parse.AppendChar(IT);}
		++I;}
		//
		TArray<FString>Comments;
		Parse.ParseIntoArray(Comments,TEXT("|~|"));
		for (const FString &C : Comments) {
			Raw.ReplaceInline(*C,TEXT(""));
		}///
	}///
	//
	//
	bool Parsed = false;
	TArray<FString>Islands;
	FClassDefinition ClassInfo;
	{///
		Raw.ParseIntoArray(Islands,TEXT(";"));
		if (Islands.Num()==0){Islands.Add(Raw);}
		//
		EAccessLevel Level = EAccessLevel::None;
		for (const FString &Island : Islands) {
			const bool _Parsed = IKMGC_ScriptParser::ParseHeaderIsland(Class,Island,ClassInfo,Level);
			if (!Parsed) {Parsed=_Parsed;}
		}///
	}///
	//
	if (ClassInfo != IKMGC_ScriptParser::NOClassInfo) {
		if (!SemanticDB->ClassDefinitions.Contains(Class)) {
			SemanticDB->ClassDefinitions.Add(Class,ClassInfo);
		} else {
			FClassDefinition &ClassRef = IKMGC_ScriptParser::GetClassReference(Class);
			if (ClassRef != IKMGC_ScriptParser::NOClassInfo) {
				ClassRef.Variables.Append(ClassInfo.Variables);
				ClassRef.Functions.Append(ClassInfo.Functions);
			}///
		}///
	}///
	//
	//
	TArray<FString>Props;
	ClassInfo.Variables.GenerateKeyArray(Props);
	for (const FString &Prop : Props) {
		FClassRedirector Redirect;
		Redirect.ObjectName = Prop;
		Redirect.ObjectClass = ClassInfo.Variables[Prop].ReturnType;
		Redirect.Hint = FString::Printf(TEXT("%s  ::  %s  %s"),*Class,*Redirect.ObjectClass,*Redirect.ObjectName);
		SemanticDB->ClassRedirectors.Add(Redirect.ObjectName,Redirect);
	}///
	//
	TArray<FString>Funs;
	ClassInfo.Functions.GenerateKeyArray(Funs);
	for (const FString &Fun : Funs) {
		FClassRedirector Redirect;
		Redirect.ObjectName = Fun;
		Redirect.ObjectClass = ClassInfo.Functions[Fun].ReturnType;
		Redirect.Hint = FString::Printf(TEXT("%s  ::  %s  %s"),*Class,*Redirect.ObjectClass,*Redirect.ObjectName);
		SemanticDB->ClassRedirectors.Add(Redirect.ObjectName,Redirect);
		FunctionDB->Extensions.Add(Redirect.ObjectName);
	}///
	//
	return Parsed;
}

const bool IKMGC_ScriptParser::ParseHeaderIsland(const FString &Class, const FString &Island, FClassDefinition &ClassInfo, EAccessLevel &Level) {
	if (Island.TrimStartAndEnd().Contains(TEXT("UFUNCTION"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().Contains(TEXT("UPROPERTY"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().Contains(TEXT("UPDATE"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().Contains(TEXT("AWAKE"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().Contains(TEXT("START"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().Contains(TEXT("STOP"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().Contains(TEXT("IMGC"),ESearchCase::CaseSensitive)) {return false;}
	if (Island.TrimStartAndEnd().IsEmpty()) {return false;}
	//
	TArray<FString>Expressions;
	Island.ParseIntoArray(Expressions,TEXT(";"));
	//
	for (const FString &Body : Expressions) {
		FPropertyDefinition Prop;
		FFunctionDefinition Fun;
		//
		uint8 Flag = INDEX_NONE;
		EType TypeOf = EType::None;
		EStack StackOf = EStack::None;
		EFunctionFlag FunType = EFunctionFlag::None;
		//
		FString Ptr, Var;
		FString Raw = Body.TrimStartAndEnd();
		Raw.ReplaceInline(TEXT("\t"),TEXT(""));
		//
		if (Raw.Contains(TEXT("protected:"),ESearchCase::CaseSensitive)) {Level=EAccessLevel::Protected; Raw.ReplaceInline(TEXT("protected:"),TEXT(""));}
		if (Raw.Contains(TEXT("private:"),ESearchCase::CaseSensitive)) {Level=EAccessLevel::Private; Raw.ReplaceInline(TEXT("private:"),TEXT(""));}
		if (Raw.Contains(TEXT("public:"),ESearchCase::CaseSensitive)) {Level=EAccessLevel::Public; Raw.ReplaceInline(TEXT("public:"),TEXT(""));}
		Raw.TrimStartAndEndInline();
		//
		Prop.Hint = FString::Printf(TEXT("%s  ::  %s"),*Class,*Raw.TrimStartAndEnd());
		Fun.Hint = FString::Printf(TEXT("%s  ::  %s"),*Class,*Raw.TrimStartAndEnd());
		//
		TArray<FString>Lines;
		Raw.ParseIntoArrayLines(Lines);
		for (FString &Line : Lines) {
			Line.Split(TEXT("::"),nullptr,&Line);
			Line.Split(TEXT("="),&Line,nullptr);
			Line.Split(TEXT("}"),&Line,nullptr);
			Line.Split(TEXT("{"),&Line,nullptr);
			Line.Split(TEXT("["),&Line,nullptr);
			Line.Split(TEXT(")"),&Line,nullptr);
			Line.TrimStartInline();
			//
			if (Line.StartsWith(TEXT("bool"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Bool; Flag=0; Prop.ReturnType=TEXT("bool");} else
			if (Line.StartsWith(TEXT("FBool"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Bool; Flag=0; Prop.ReturnType=TEXT("bool");} else
			//
			if (Line.StartsWith(TEXT("float"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Float; Flag=0; Prop.ReturnType=TEXT("float");} else
			if (Line.StartsWith(TEXT("FFloat"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Float; Flag=0; Prop.ReturnType=TEXT("float");} else
			//
			if (Line.StartsWith(TEXT("double"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Float; Flag=0; Prop.ReturnType=TEXT("double");} else
			if (Line.StartsWith(TEXT("FDouble"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Float; Flag=0; Prop.ReturnType=TEXT("double");} else
			//
			if (Line.StartsWith(TEXT("uint8"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("uint8");} else
			if (Line.StartsWith(TEXT("FUByte"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("uint8");} else
			if (Line.StartsWith(TEXT("uint16"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("uint16");} else
			if (Line.StartsWith(TEXT("FUShort"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("uint16");} else
			if (Line.StartsWith(TEXT("uint32"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("uint32");} else
			if (Line.StartsWith(TEXT("FUInt"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("uint32");} else
			if (Line.StartsWith(TEXT("uint64"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("uint64");} else
			if (Line.StartsWith(TEXT("FULong"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("uint64");} else
			//
			if (Line.StartsWith(TEXT("int8"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("int8");} else
			if (Line.StartsWith(TEXT("FByte"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("int8");} else
			if (Line.StartsWith(TEXT("int16"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("int16");} else
			if (Line.StartsWith(TEXT("FShort"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Byte; Flag=0; Prop.ReturnType=TEXT("int16");} else
			if (Line.StartsWith(TEXT("int32"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("int32");} else
			if (Line.StartsWith(TEXT("FInt"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("int32");} else
			if (Line.StartsWith(TEXT("int64"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("int64");} else
			if (Line.StartsWith(TEXT("FLong"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Int; Flag=0; Prop.ReturnType=TEXT("int64");} else
			//
			if (Line.StartsWith(TEXT("FText"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Text; Flag=0; Prop.ReturnType=TEXT("FText");} else
			if (Line.StartsWith(TEXT("FName"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::Name; Flag=0; Prop.ReturnType=TEXT("FName");} else
			if (Line.StartsWith(TEXT("FString"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(" "),&Ptr,&Var); TypeOf=EType::String; Flag=0; Prop.ReturnType=TEXT("FString");} else
			//
			if (Line.StartsWith(TEXT("TArray"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(">"),&Ptr,&Var); Ptr.AppendChar(TEXT('>')); StackOf=EStack::Array; Flag=0; Prop.ReturnType=TEXT("TArra<>");} else
			if (Line.StartsWith(TEXT("TSet"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(">"),&Ptr,&Var); Ptr.AppendChar(TEXT('>'));  StackOf=EStack::Set; Flag=0; Prop.ReturnType=TEXT("TSet<>");} else
			if (Line.StartsWith(TEXT("TMap"),ESearchCase::CaseSensitive)) {Line.Split(TEXT(">"),&Ptr,&Var); Ptr.AppendChar(TEXT('>'));  StackOf=EStack::Map; Flag=0; Prop.ReturnType=TEXT("TMap<>");} else
			//
			if (Line.Contains(TEXT("*")) && !Line.Contains(TEXT("("))) {
				TypeOf=EType::Object; Flag=0;
				Line.Split(TEXT("*"),&Ptr,&Var);
				Fun.ReturnType=Ptr.TrimStartAndEnd();
				Prop.ReturnType=Ptr.TrimStartAndEnd();
			} else if (Line.Contains(TEXT("&")) && !Line.Contains(TEXT("("))) {
				TypeOf=EType::Object; Flag=0;
				Line.Split(TEXT("&"),&Ptr,&Var);
				Fun.ReturnType=Ptr.TrimStartAndEnd();
				Prop.ReturnType=Ptr.TrimStartAndEnd();
			} else if (Line.TrimStart().StartsWith(TEXT("E"),ESearchCase::CaseSensitive) && !Line.Contains(TEXT("("))) {
				TypeOf=EType::Enum; Flag=0;
				Line.Split(TEXT(" "),&Ptr,&Var);
				Fun.ReturnType=Ptr.TrimStartAndEnd();
				Prop.ReturnType=Ptr.TrimStartAndEnd();
			} else if (Line.TrimStart().StartsWith(TEXT("F"),ESearchCase::CaseSensitive) && !Line.Contains(TEXT("("))) {
				TypeOf=EType::Struct; Flag=0;
				Line.Split(TEXT(" "),&Ptr,&Var);
				Fun.ReturnType=Ptr.TrimStartAndEnd();
				Prop.ReturnType=Ptr.TrimStartAndEnd();
			}///
			//
			if (Line.Contains(TEXT("("))) {
				Line.Split(TEXT("("),&Line,nullptr);
				Line.TrimStartAndEndInline();
				FunType=EFunctionFlag::Native; Flag=1;
				//
				if (Line.Split(TEXT(">"),&Ptr,&Var)) {
					Ptr.AppendChar(TEXT('>'));
				} else {Line.Split(TEXT(" "),&Ptr,&Var);}
				//
				Fun.ReturnType = Ptr.TrimStartAndEnd();
			}///
			//
			Var.TrimStartAndEndInline();
		}///
		//
		if (Flag==0) {
			Prop.Access = Level;
			Prop.TypeOf = TypeOf;
			Prop.StackOf = StackOf;
			ClassInfo.Variables.Add(Var,Prop);
		} else if (Flag==1) {
			Fun.Flag = FunType;
			Fun.Access = Level;
			Fun.TypeOf = TypeOf;
			Fun.StackOf = StackOf;
			ClassInfo.Functions.Add(Var,Fun);
		}///
	}///
	//
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////