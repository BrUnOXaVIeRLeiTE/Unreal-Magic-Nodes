//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MagicNodeRuntime.h"
#include "MagicNodeRuntime_Shared.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Object:: Native Functions

UMagicNodeScript::UMagicNodeScript() {
#if WITH_EDITOR
	RuntimeScriptClass = UMagicNode::StaticClass();
	ParentClass = UMagicNode::StaticClass();
	//
	NodeColor = FLinearColor(FColor(5,15,25));
	//
	//
if (Source.Script.IsEmpty()) {
	Source.Header = TEXT("public:\n\nIMGC() static void Execute(UObject* Context);\n\n\n\n\n\n\n");
	Source.Script =
TEXT("\
void FSelf::Execute(UObject* Context)\n\
{\n\
	Ensure(Context);\n\
	UE_LOG(LogTemp,Log,TEXT(\"%s\"),TEXT(\"Hello World!\"));\n\
}\n\n\n\n\n\
");///
}/////
	//
	//
	LockSourceCode = false;
#endif
}

void UMagicNodeScript::PostInitProperties() {
	Super::PostInitProperties();
	//
	if (!HasAnyFlags(RF_ClassDefaultObject)) {return;}
	//
	RefreshRuntimeScriptClass();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Script:: Kismet Interface

const bool UMagicNodeScript::RefreshRuntimeScriptClass() {
#if WITH_EDITOR
	FString Name = GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	Name.RemoveFromStart(TEXT("Default__"));
	//
	for (TObjectIterator<UClass>IT; IT; ++IT) {
		if (!IT->IsChildOf<UMagicNode>()) {continue;}
		if (!IT->GetName().Contains(TEXT("__CPP"))) {continue;}
		//
		FString Compared = IT->GetName();
		Compared.RemoveFromEnd(TEXT("__CPP"));
		//
		if (Name==Compared) {
			RuntimeScriptClass = *IT;
		break;}
	}///
	//
	if (RuntimeScriptClass.Get()!=nullptr) {return true;}
#endif
	//
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Object:: Runtime Interface

UMagicNode::UMagicNode() {
	TickRate = 0.001f;
	TickDelta = 0.123f;
	//
	World = nullptr;
	CanTick = false;
}

void UMagicNode::Run() {
	if (World==nullptr||HasAnyFlags(RF_ArchetypeObject|RF_ClassDefaultObject)) {return;}
	//
	TickRate = World->GetDeltaSeconds();
	Awake();
	//
	if (CanTick) {
		const FTimerDelegate TimerDelegate = FTimerDelegate::CreateUObject(this,&UMagicNode::Tick);
		World->GetTimerManager().SetTimer(UpdateHandler,TimerDelegate,TickRate,true);
	}///
	//
	Start();
}

void UMagicNode::Finish() {
	if (World==nullptr) {return;}
	//
	World->GetTimerManager().ClearAllTimersForObject(this);
	if (!HasAnyFlags(RF_ArchetypeObject|RF_ClassDefaultObject)) {ConditionalBeginDestroy();}
}

void UMagicNode::BeginDestroy() {
	if (World==nullptr||HasAnyFlags(RF_ArchetypeObject|RF_ClassDefaultObject)) {
		Super::BeginDestroy();
	return;}
	//
	Stop();
	//
	Super::BeginDestroy();
}

UWorld* UMagicNode::GetWorld() const {
	return World;
}

void UMagicNode::Tick() {
	if (World==nullptr||HasAnyFlags(RF_ArchetypeObject|RF_ClassDefaultObject)) {Finish(); return;}
	//
	TickDelta = World->GetDeltaSeconds();
	//
	Update(TickDelta);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////