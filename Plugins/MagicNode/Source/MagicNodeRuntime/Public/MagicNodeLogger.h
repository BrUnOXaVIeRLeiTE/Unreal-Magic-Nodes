//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"

#include "MagicNodeLogger.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DECLARE_LOG_CATEGORY_EXTERN(MagicNodes,Log,All);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UENUM()
enum class EMGCSeverity : uint8 {
	Fatal			= 0,
	Error			= 1,
	Warning			= 2,
	Info			= 3
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMagicNodeScript;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MAGICNODERUNTIME_API void LOG_MGC(const FName Message);
MAGICNODERUNTIME_API void LOG_MGC(const TCHAR* Message);
MAGICNODERUNTIME_API void LOG_MGC(const FString Message);
//
MAGICNODERUNTIME_API void LOG_MGC(const EMGCSeverity Severity, const FName Message);
MAGICNODERUNTIME_API void LOG_MGC(const EMGCSeverity Severity, const TCHAR* Message);
MAGICNODERUNTIME_API void LOG_MGC(const EMGCSeverity Severity, const FString Message);
//
MAGICNODERUNTIME_API void LOG_MGC(const float Duration, const FColor Color, const FName Message);
MAGICNODERUNTIME_API void LOG_MGC(const float Duration, const FColor Color, const TCHAR* Message);
MAGICNODERUNTIME_API void LOG_MGC(const float Duration, const FColor Color, const FString Message);
//
MAGICNODERUNTIME_API void LOG_BP(const UFunction* Function);
MAGICNODERUNTIME_API void LOG_BP(const UFunction* Function, const UObject* Owner);
//
MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const FName Message);
MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const TCHAR* Message);
MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const FString Message);
//
MAGICNODERUNTIME_API void LOG_BP(const UObject* Owner, const UFunction* Function, const FString Message = TEXT(""));
MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const UObject* Owner, const UFunction* Function, const FString Message = TEXT(""));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct __MGC_Debug__ {
	MAGICNODERUNTIME_API void Log(const FText Message);
	MAGICNODERUNTIME_API void Log(const FName Message);
	MAGICNODERUNTIME_API void Log(const TCHAR* Message);
	MAGICNODERUNTIME_API void Log(const FString Message);
	//
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const FText Message);
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const FName Message);
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const TCHAR* Message);
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const FString Message);
	//
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const FText Message);
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const FName Message);
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const TCHAR* Message);
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const FString Message);
	//
	//
	MAGICNODERUNTIME_API void Log(const uint8 Bytes);
	MAGICNODERUNTIME_API void Log(const float Floats);
	MAGICNODERUNTIME_API void Log(const int32 Integer);
	//
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const uint8 Bytes);
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const float Floats);
	MAGICNODERUNTIME_API void Log(const EMGCSeverity Severity, const int32 Integer);
	//
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const uint8 Bytes);
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const float Floats);
	MAGICNODERUNTIME_API void Print(const float Duration, const FColor Color, const int32 Integer);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////