//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeRuntime.h"
#include "MagicNodeKismet_Shared.h"
#include "MagicNodeKismetSettings.h"

#include "Runtime/Core/Public/Async/AsyncWork.h"
#include "Runtime/Core/Public/Async/TaskGraphInterfaces.h"
#include "Runtime/AssetRegistry/Public/AssetRegistryModule.h"
#include "Runtime/Projects/Public/Interfaces/IPluginManager.h"

#include "KMGC_KismetTypes.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Parser Database Types:

enum class EDatabaseState : uint8 {
	READY,
	ASYNCLOADING
};

UENUM()
enum class EType : uint8 {
	None,
	Int,
	Byte,
	Bool,
	Float,
	Name,
	Text,
	String,
	Enum,
	Class,
	Struct,
	Object
};

UENUM()
enum class EStack : uint8 {
	None,
	Array,
	Set,
	Map
};

UENUM()
enum class EFunctionFlag : uint8 {
	None,
	Pure,
	Const,
	Event,
	Native,
	Callable,
	Delegate
};

UENUM()
enum class EAccessLevel : uint8 {
	None,
	Public,
	Private,
	Protected,
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
	UPROPERTY() EStack StackOf;
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
			case EType::Int: return TEXT("int32"); break;
			case EType::Bool: return TEXT("bool"); break;
			case EType::Byte: return TEXT("uint8"); break;
			case EType::Name: return TEXT("FName"); break;
			case EType::Text: return TEXT("FText"); break;
			case EType::Float: return TEXT("float"); break;
			case EType::String: return TEXT("FString"); break;
			//
			case EType::Enum: return TEXT("UEnum"); break;
			case EType::Class: return TEXT("UClass"); break;
			case EType::Struct: return TEXT("UStruct"); break;
			case EType::Object: return TEXT("UObject"); break;
		default: break;}
		//
		return TEXT("<?>");
	}///
	//
	FString StackToString() const {
		switch (StackOf) {
			case EStack::Map: return TEXT("TMap"); break;
			case EStack::Set: return TEXT("TSet"); break;
			case EStack::Array: return TEXT("TArray"); break;
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
	UPROPERTY() EStack StackOf;
	UPROPERTY() EFunctionFlag Flag;
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
		switch (Flag) {
			case EFunctionFlag::Pure: return TEXT("Pure"); break;
			case EFunctionFlag::Const: return TEXT("Const"); break;
			case EFunctionFlag::Event: return TEXT("Event"); break;
			case EFunctionFlag::Native: return TEXT("Native"); break;
			case EFunctionFlag::Delegate: return TEXT("Event"); break;
			case EFunctionFlag::Callable: return TEXT("Callable"); break;
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

UCLASS(classGroup="Synaptech", Category="Synaptech")
class MAGICNODEKISMET_API UMGC_KeywordDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_KeywordDB();
public:
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
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>Extensions;
};

UCLASS(classGroup="Synaptech", Category="Synaptech")
class MAGICNODEKISMET_API UMGC_ClassDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_ClassDB();
public:
	/* MGC Default Scripting Classes. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptCore;
	//
	/* MGC Default Type Definitions. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptTypes;
	//
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>Extensions;
};

UCLASS(classGroup="Synaptech", Category="Synaptech")
class MAGICNODEKISMET_API UMGC_FunctionDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_FunctionDB();
public:
	/* MGC Default Scripting Classes. */
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>ScriptCore;
	//
	UPROPERTY(Category="Keywords", VisibleAnywhere)
	TSet<FString>Extensions;
};

UCLASS(classGroup="Synaptech", Category="Synaptech")
class MAGICNODEKISMET_API UMGC_SemanticDB : public UObject {
	GENERATED_BODY()
	//
	UMGC_SemanticDB();
protected:
	bool IsValidTarget(UClass* Class) const;
public:
	void UpdateExtensions();
	void RegisterEnumReflection(UEnum* Enum);
	void RegisterStructReflection(UStruct* Struct);
	void RegisterClassReflection(UClass* Class, FString Prefix);
	//
	FString PropertyToTypeName(TFieldPath<FProperty>Property);
	//
	//
	/* MGC Default Scripting Keyword Definitions. */
	UPROPERTY(Category="Definitions", VisibleAnywhere)
	TMap<FString,FKeywordDefinition>KeywordDefinitions;
	//
	/* MGC Default Scripting Type Definitions. */
	UPROPERTY(Category="Definitions", VisibleAnywhere)
	TMap<FString,FKeywordDefinition>TypeDefinitions;
	//
	/* MGC Default Scripting Macro Definitions. */
	UPROPERTY(Category="Definitions", VisibleAnywhere)
	TMap<FString,FKeywordDefinition>MacroDefinitions;
	//
	/* MGC Default Scripting Class Definitions. */
	UPROPERTY(Category="Definitions", VisibleAnywhere)
	TMap<FString,FClassDefinition>ClassDefinitions;
	//
	/* MGC Default Scripting Map of Instantiated Classes. */
	UPROPERTY(Category="Definitions", VisibleAnywhere)
	TMap<FString,FClassRedirector>ClassRedirectors;
	//
	/* MGC Important Classes' Documentation. */
	UPROPERTY(Category="API Reference", VisibleAnywhere)
	TMap<FString,FString>Documentation;
public:
	static EDatabaseState DBState;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TASK_BuildAutoCompleteData : public FNonAbandonableTask {
private:
	UMGC_SemanticDB* DB;
public:
	TASK_BuildAutoCompleteData(UMGC_SemanticDB* InDB){DB=InDB;}
	//
	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(TASK_BuildAutoCompleteData,STATGROUP_ThreadPoolAsyncTasks);
	}///
	//
	void DoWork() {DB->UpdateExtensions();}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////