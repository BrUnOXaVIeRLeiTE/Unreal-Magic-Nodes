//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeRuntime.h"
#include "MagicNodeKismet_Shared.h"
#include "MagicNodeKismetSettings.h"

#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"

#include "KMGC_KismetTypes.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Parser Database Types:

UENUM()
enum class EType : uint8 {
	None,
	Array,
	Set,
	Map,
	Native,
	Callable,
	Pure,
	Const,
	Event,
	Delegate,
	Integer,
	Byte,
	Bool,
	Float,
	Name,
	Text,
	String,
	Enum,
	Struct,
	Object
};

UENUM()
enum class EAccessLevel : uint8 {
	None,
	Public,
	Protected,
	Private,
	Static
};

USTRUCT()
struct FKeywordDefinition {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY(Category="Definitions", EditDefaultsOnly) FString Info;
	//
	//
	friend inline uint32 GetTypeHash(const FKeywordDefinition &DEF) {
		return FCrc::MemCrc32(&DEF,sizeof(FKeywordDefinition));
	}///
	//
	bool operator == (const FKeywordDefinition &Other) const {
		return (
			Info == Other.Info
		);///
	}///
	//
	bool operator != (const FKeywordDefinition &Other) const {
		return !(*this==Other);
	}///
};

USTRUCT()
struct FPropertyDefinition {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY() EType TypeOf;
	UPROPERTY() EAccessLevel Access;
	//
	UPROPERTY() FString ReturnType;
	UPROPERTY() FString Tooltip;
	UPROPERTY() FString Hint;
	//
	//
	friend inline uint32 GetTypeHash(const FPropertyDefinition &DEF) {
		return FCrc::MemCrc32(&DEF,sizeof(FPropertyDefinition));
	}///
	//
	bool operator == (const FPropertyDefinition &Other) const {
		return (
			Tooltip == Other.Tooltip &&
			TypeOf == Other.TypeOf &&
			Access == Other.Access &&
			Hint == Other.Hint
		);///
	}///
	//
	bool operator != (const FPropertyDefinition &Other) const {
		return !(*this==Other);
	}///
	//
	//
	FString TypeToString() const {
		switch (TypeOf) {
			case EType::Bool: return TEXT("bool"); break;
			case EType::Name: return TEXT("FName"); break;
			case EType::Text: return TEXT("FText"); break;
			case EType::Byte: return TEXT("uint8"); break;
			case EType::Enum: return TEXT("UEnum"); break;
			case EType::Float: return TEXT("float"); break;
			case EType::Integer: return TEXT("int32"); break;
			case EType::String: return TEXT("FString"); break;
			case EType::Object: return TEXT("UObject"); break;
			case EType::Struct: return TEXT("UScriptStruct"); break;
		default: break;}
		//
		return TEXT("<?>");
	}///
	//
	FString AccessToString() const {
		switch (Access) {
			case EAccessLevel::Protected: return TEXT("Protected"); break;
			case EAccessLevel::Private: return TEXT("Private"); break;
			case EAccessLevel::Public: return TEXT("Public"); break;
			case EAccessLevel::Static: return TEXT("Static"); break;
		default: break;}
		//
		return TEXT("<?>");
	}///
};

USTRUCT()
struct FFunctionDefinition {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY() EType TypeOf;
	UPROPERTY() EAccessLevel Access;
	//
	UPROPERTY() FString ReturnType;
	UPROPERTY() TSet<FString>Inputs;
	UPROPERTY() TSet<FString>Outputs;
	//
	UPROPERTY() FString Hint;
	UPROPERTY() FString Tooltip;
	//
	//
	friend inline uint32 GetTypeHash(const FFunctionDefinition &DEF) {
		return FCrc::MemCrc32(&DEF,sizeof(FFunctionDefinition));
	}///
	//
	bool operator == (const FFunctionDefinition &Other) const {
		return (
			Outputs.GetAllocatedSize() == Other.Outputs.GetAllocatedSize() &&
			Inputs.GetAllocatedSize() == Other.Inputs.GetAllocatedSize() &&
			ReturnType == Other.ReturnType &&
			//
			TypeOf == Other.TypeOf &&
			Access == Other.Access &&
			//
			Tooltip == Other.Tooltip &&
			Hint == Other.Hint
		);///
	}///
	//
	bool operator != (const FFunctionDefinition &Other) const {
		return !(*this==Other);
	}///
	//
	//
	FString TypeToString() const {
		switch (TypeOf) {
			case EType::Pure: return TEXT("Pure"); break;
			case EType::Const: return TEXT("Const"); break;
			case EType::Event: return TEXT("Event"); break;
			case EType::Native: return TEXT("Native"); break;
			case EType::Delegate: return TEXT("Event"); break;
			case EType::Callable: return TEXT("Callable"); break;
		default: break;}
		//
		return TEXT("<?>");
	}///
	//
	FString AccessToString() const {
		switch (Access) {
			case EAccessLevel::Protected: return TEXT("Protected"); break;
			case EAccessLevel::Private: return TEXT("Private"); break;
			case EAccessLevel::Public: return TEXT("Public"); break;
			case EAccessLevel::Static: return TEXT("Static"); break;
		default: break;}
		//
		return TEXT("<?>");
	}///
};

USTRUCT()
struct FClassDefinition {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY() TMap<FString,FPropertyDefinition>Variables;
	UPROPERTY() TMap<FString,FFunctionDefinition>Functions;
	//
	UPROPERTY() FString ParentClass;
	UPROPERTY() FString Tooltip;
	UPROPERTY() FString Hint;
	//
	//
	friend inline uint32 GetTypeHash(const FClassDefinition &DEF) {
		return FCrc::MemCrc32(&DEF,sizeof(FClassDefinition));
	}///
	//
	bool operator == (const FClassDefinition &Other) const {
		return (
			Variables.GetAllocatedSize() == Other.Variables.GetAllocatedSize() &&
			Functions.GetAllocatedSize() == Other.Functions.GetAllocatedSize() &&
			//
			ParentClass == Other.ParentClass &&
			Tooltip == Other.Tooltip &&
			Hint == Other.Hint
		);///
	}///
	//
	bool operator != (const FClassDefinition &Other) const {
		return !(*this==Other);
	}///
};

USTRUCT()
struct FClassRedirector {
	GENERATED_USTRUCT_BODY()
	//
	UPROPERTY() FString ObjectClass;
	UPROPERTY() FString OwnerClass;
	UPROPERTY() FString ObjectName;
	UPROPERTY() FString Hint;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Parser Database Classes:

UCLASS(classGroup=Synaptech, Category="Synaptech")
class MAGICNODEKISMET_API UMGC_KeywordDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_KeywordDB();
public:
	void PostLoad() override;
	void UpdateExtensions();
	//
	//
	/* MGC Default Scripting Keywords. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptCore;
	//
	/* MGC Default Operator Keywords. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>Operators;
	//
	/* MGC Default Pre-Processor Keywords. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>Processors;
	//
	/* MGC Default Macro Keywords. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>Macros;
	//
	UPROPERTY(Category="Keywords", EditAnywhere)
	TSet<FString>Extensions;
};

UCLASS(classGroup=Synaptech, Category="Synaptech")
class MAGICNODEKISMET_API UMGC_ClassDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_ClassDB();
public:
	void PostLoad() override;
	void UpdateExtensions();
	//
	//
	/* MGC Default Scripting Classes. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptCore;
	//
	/* MGC Default Type Definitions. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptTypes;
	//
	UPROPERTY(Category="Keywords", EditAnywhere)
	TSet<FString>Extensions;
};

UCLASS(classGroup=Synaptech, Category="Synaptech")
class MAGICNODEKISMET_API UMGC_FunctionDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_FunctionDB();
public:
	void PostLoad() override;
	void UpdateExtensions();
	//
	//
	/* MGC Default Scripting Classes. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptCore;
	//
	UPROPERTY(Category="Keywords", EditAnywhere)
	TSet<FString>Extensions;
};

UCLASS(classGroup=Synaptech, Category="Synaptech")
class MAGICNODEKISMET_API UMGC_SemanticDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_SemanticDB();
protected:
	bool IsValidTarget(const UClass* Class) const;
public:
	void UpdateExtensions();
	void RegisterClassReflection(const UClass* Class, const TCHAR* Prefix);
	const FString PropertyToTypeName(const UProperty* Property, const UObject* Container);
	//
	//
	/* MGC Default Scripting Keyword Definitions. */
	UPROPERTY()
	TMap<FString,FKeywordDefinition>KeywordDefinitions;
	//
	/* MGC Default Scripting Type Definitions. */
	UPROPERTY()
	TMap<FString,FKeywordDefinition>TypeDefinitions;
	//
	/* MGC Default Scripting Macro Definitions. */
	UPROPERTY()
	TMap<FString,FKeywordDefinition>MacroDefinitions;
	//
	/* MGC Default Scripting Class Definitions. */
	UPROPERTY()
	TMap<FString,FClassDefinition>ClassDefinitions;
	//
	/* MGC Default Scripting Map of Instantiated Classes. */
	UPROPERTY()
	TMap<FString,FClassRedirector>ClassRedirectors;
	//
	/* MGC Important Classes' Documentation. */
	UPROPERTY(Category="API Reference", EditAnywhere)
	TMap<FString,FString>Documentation;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////