//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MagicNodeLogger.h"

#if WITH_EDITOR
	#include "Logging/MessageLog.h"
	#include "Logging/TokenizedMessage.h"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DEFINE_LOG_CATEGORY(MagicNodes);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


MAGICNODERUNTIME_API void LOG_MGC(const TCHAR* Message) {
	UE_LOG(MagicNodes,Warning,TEXT("%s"),Message);
}

MAGICNODERUNTIME_API void LOG_MGC(const FName Message) {
	UE_LOG(MagicNodes,Warning,TEXT("%s"),*Message.ToString());
}

MAGICNODERUNTIME_API void LOG_MGC(const FString Message) {
	UE_LOG(MagicNodes,Warning,TEXT("%s"),*Message);
}

MAGICNODERUNTIME_API void LOG_MGC(const EMGCSeverity Severity, const TCHAR* Message) {
	switch (Severity) {
		case EMGCSeverity::Info:
			UE_LOG(MagicNodes,Log,TEXT("%s"),Message); break;
		case EMGCSeverity::Warning:
			UE_LOG(MagicNodes,Warning,TEXT("%s"),Message); break;
		case EMGCSeverity::Error:
			UE_LOG(MagicNodes,Error,TEXT("%s"),Message); break;
		case EMGCSeverity::Fatal:
			UE_LOG(MagicNodes,Fatal,TEXT("%s"),Message); break;
	default: break;}
}

MAGICNODERUNTIME_API void LOG_MGC(const EMGCSeverity Severity, const FName Message) {
	switch (Severity) {
		case EMGCSeverity::Info:
			UE_LOG(MagicNodes,Log,TEXT("%s"),*Message.ToString()); break;
		case EMGCSeverity::Warning:
			UE_LOG(MagicNodes,Warning,TEXT("%s"),*Message.ToString()); break;
		case EMGCSeverity::Error:
			UE_LOG(MagicNodes,Error,TEXT("%s"),*Message.ToString()); break;
		case EMGCSeverity::Fatal:
			UE_LOG(MagicNodes,Fatal,TEXT("%s"),*Message.ToString()); break;
	default: break;}
}

MAGICNODERUNTIME_API void LOG_MGC(const EMGCSeverity Severity, const FString Message) {
	switch (Severity) {
		case EMGCSeverity::Info:
			UE_LOG(MagicNodes,Log,TEXT("%s"),*Message); break;
		case EMGCSeverity::Warning:
			UE_LOG(MagicNodes,Warning,TEXT("%s"),*Message); break;
		case EMGCSeverity::Error:
			UE_LOG(MagicNodes,Error,TEXT("%s"),*Message); break;
		case EMGCSeverity::Fatal:
			UE_LOG(MagicNodes,Fatal,TEXT("%s"),*Message); break;
	default: break;}
}

MAGICNODERUNTIME_API void LOG_MGC(const float Duration, const FColor Color, const TCHAR* Message) {
	if (GEngine) {GEngine->AddOnScreenDebugMessage(-1,Duration,Color,FString::Printf(TEXT("{MGC}:: %s"),Message));}
}

MAGICNODERUNTIME_API void LOG_MGC(const float Duration, const FColor Color, const FName Message) {
	if (GEngine) {GEngine->AddOnScreenDebugMessage(-1,Duration,Color,FString::Printf(TEXT("{MGC}:: %s"),*Message.ToString()));}
}

MAGICNODERUNTIME_API void LOG_MGC(const float Duration, const FColor Color, const FString Message) {
	if (GEngine) {GEngine->AddOnScreenDebugMessage(-1,Duration,Color,FString::Printf(TEXT("{MGC}:: %s"),*Message));}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const UObject* Owner, const UFunction* Function, const FString Message) {
	if (!Function) {return;} if (!Owner) {return;}
	//
#if WITH_EDITOR
	FFormatNamedArguments ARG;
	FFormatArgumentValue AMessage = FText::FromString(Message);
	FFormatArgumentValue AFunction = Function->GetDisplayNameText();
	FFormatArgumentValue AInfo = FText::FromString(Function->GetFullName());
	FFormatArgumentValue APackage = FText::FromString(Owner->GetFullName());
	//
	ARG.Add(TEXT("Details"),AInfo);
	ARG.Add(TEXT("Message"),AMessage);
	ARG.Add(TEXT("Package"),APackage);
	ARG.Add(TEXT("Function"),AFunction);
	const auto Token = FTextToken::Create(FText::Format(LOCTEXT("LOG_BP","{Function}: [{Details} at ({Package})]"),ARG));
	//
	switch (Severity) {
		case EMGCSeverity::Info:
			FMessageLog("PIE").Message(EMessageSeverity::Info,LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Warning:
			FMessageLog("PIE").Warning(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Error:
			FMessageLog("PIE").Error(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Fatal:
			FMessageLog("PIE").CriticalError(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
	default: break;}
#endif
}

MAGICNODERUNTIME_API void LOG_BP(const UObject* Owner, const UFunction* Function, const FString Message) {
	LOG_BP(EMGCSeverity::Warning,Owner,Function,Message);
}

MAGICNODERUNTIME_API void LOG_BP(const UFunction* Function, const UObject* Owner) {
	LOG_BP(Owner,Function,FString(TEXT("Something went wrong!")));
}

MAGICNODERUNTIME_API void LOG_BP(const UFunction* Function) {
	LOG_BP(EMGCSeverity::Info,Function,Function,TEXT(""));
}

MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const FName Message) {
#if WITH_EDITOR
	FFormatNamedArguments ARG;
	FFormatArgumentValue AMessage = FText::FromString(Message.ToString());
	//
	ARG.Add(TEXT("Message"),AMessage);
	const auto Token = FTextToken::Create(FText::Format(LOCTEXT("LOG_BP","{Message}"),ARG));
	//
	switch (Severity) {
		case EMGCSeverity::Info:
			FMessageLog("PIE").Message(EMessageSeverity::Info,LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Warning:
			FMessageLog("PIE").Warning(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Error:
			FMessageLog("PIE").Error(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Fatal:
			FMessageLog("PIE").CriticalError(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
	default: break;}
#endif
}

MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const TCHAR* Message) {
#if WITH_EDITOR
	FFormatNamedArguments ARG;
	FFormatArgumentValue AMessage = FText::FromString(FString(Message));
	//
	ARG.Add(TEXT("Message"),AMessage);
	const auto Token = FTextToken::Create(FText::Format(LOCTEXT("LOG_BP","{Message}"),ARG));
	//
	switch (Severity) {
		case EMGCSeverity::Info:
			FMessageLog("PIE").Message(EMessageSeverity::Info,LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Warning:
			FMessageLog("PIE").Warning(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Error:
			FMessageLog("PIE").Error(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Fatal:
			FMessageLog("PIE").CriticalError(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
	default: break;}
#endif
}

MAGICNODERUNTIME_API void LOG_BP(const EMGCSeverity Severity, const FString Message) {
#if WITH_EDITOR
	FFormatNamedArguments ARG;
	FFormatArgumentValue AMessage = FText::FromString(Message);
	//
	ARG.Add(TEXT("Message"),AMessage);
	const auto Token = FTextToken::Create(FText::Format(LOCTEXT("LOG_BP","{Message}"),ARG));
	//
	switch (Severity) {
		case EMGCSeverity::Info:
			FMessageLog("PIE").Message(EMessageSeverity::Info,LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Warning:
			FMessageLog("PIE").Warning(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Error:
			FMessageLog("PIE").Error(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
		case EMGCSeverity::Fatal:
			FMessageLog("PIE").CriticalError(LOCTEXT("LOG_MGC","{MGC}: "))->AddToken(Token); break;
	default: break;}
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void __MGC_Debug__::Log(const FName Message) {
	LOG_MGC(Message);
}

void __MGC_Debug__::Log(const TCHAR* Message) {
	LOG_MGC(Message);
}

void __MGC_Debug__::Log(const FString Message) {
	LOG_MGC(Message);
}

void __MGC_Debug__::Log(const FText Message) {
	LOG_MGC(Message.ToString());
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const FName Message) {
	LOG_MGC(Severity,Message);
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const TCHAR* Message) {
	LOG_MGC(Severity,Message);
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const FString Message) {
	LOG_MGC(Severity,Message);
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const FText Message) {
	LOG_MGC(Severity,Message.ToString());
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const FName Message) {
	LOG_MGC(Duration,Color,Message);
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const TCHAR* Message) {
	LOG_MGC(Duration,Color,Message);
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const FString Message) {
	LOG_MGC(Duration,Color,Message);
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const FText Message) {
	LOG_MGC(Duration,Color,Message.ToString());
}

void __MGC_Debug__::Log(const uint8 Bytes) {
	const FString B = FString::Printf(TEXT("%i"),Bytes);
	//
	LOG_MGC(B);
}

void __MGC_Debug__::Log(const int32 Integer) {
	const FString I = FString::Printf(TEXT("%i"),Integer);
	//
	LOG_MGC(I);
}

void __MGC_Debug__::Log(const float Floats) {
	const FString F = FString::Printf(TEXT("%f"),Floats);
	//
	LOG_MGC(F);
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const uint8 Bytes) {
	const FString B = FString::Printf(TEXT("%i"),Bytes);
	//
	LOG_MGC(Severity,B);
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const int32 Integer) {
	const FString I = FString::Printf(TEXT("%i"),Integer);
	//
	LOG_MGC(Severity,I);
}

void __MGC_Debug__::Log(const EMGCSeverity Severity, const float Floats) {
	const FString F = FString::Printf(TEXT("%f"),Floats);
	//
	LOG_MGC(Severity,F);
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const uint8 Bytes) {
	const FString B = FString::Printf(TEXT("%i"),Bytes);
	//
	LOG_MGC(Duration,Color,B);
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const int32 Integer) {
	const FString I = FString::Printf(TEXT("%i"),Integer);
	//
	LOG_MGC(Duration,Color,I);
}

void __MGC_Debug__::Print(const float Duration, const FColor Color, const float Floats) {
	const FString F = FString::Printf(TEXT("%f"),Floats);
	//
	LOG_MGC(Duration,Color,F);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////