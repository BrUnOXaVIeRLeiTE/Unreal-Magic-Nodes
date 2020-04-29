//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeLogger.h"
#include "MagicNodeSettings.h"
#include "MagicNodeDataTypes.h"
#include "MagicNodeRuntime_Shared.h"

#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"

#include "Runtime/Slate/Public/SlateBasics.h"
#include "Runtime/Slate/Public/SlateExtras.h"
#include "Runtime/SlateCore/Public/SlateCore.h"

#include "MagicNodeRuntime.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define Ensure(OBJ) if (!ensureMsgf((OBJ!=NULL)&&(OBJ->IsValidLowLevel()),TEXT("{MGC}: Just checked and it doesn't seem to be a valid pointer to an object!"))) {return;}
#define Enroll(_Context,_Type) MGC_Allocate<_Type>(_Context,_Type::StaticClass())

#define UPDATE() virtual void Update(float DeltaTime) override
#define START() virtual void Start() override
#define AWAKE() virtual void Awake() override
#define STOP() virtual void Stop() override

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMGC;
class UMagicNode;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Runtime:: Node Classes

/// Magic-Node's Runtime Object.
UCLASS(classGroup="Synaptech", Category="Magic Node", BlueprintType, hideCategories=("Activation","Variable"))
class MAGICNODERUNTIME_API UMagicNode : public UObject {
	GENERATED_BODY()
public:
	UMagicNode();
protected:
	__MGC_Debug__ Debug;
protected:
	bool CanTick;
	float TickRate;
	float TickDelta;
	//
	UWorld* World;
	FTimerHandle UpdateHandler;
	//
	void Run();
	void Tick();
	//
	virtual void Stop(void){}
	virtual void Awake(void){}
	virtual void Start(void){}
	virtual void Update(float DeltaTime){}
public:
	virtual UWorld* GetWorld() const override;
	virtual void BeginDestroy() override;
	//
	void RegisterWorld(UWorld* NewWorld) {World=NewWorld;}
	//
	UFUNCTION(Category="Magic Node",BlueprintCallable)
	void Finish();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Object:: Node Script

/* C++ Scriptable Node for Blueprint Graphs */
UCLASS(classGroup="Synaptech", Category="Magic Node", Blueprintable, hideCategories=("Activation","Variable"), meta=(DisplayName="Magic Node Script"))
class MAGICNODERUNTIME_API UMagicNodeScript : public UObject {
	GENERATED_BODY()
	//
	UMagicNodeScript();
protected:
	UPROPERTY()
	TSubclassOf<UMagicNode>RuntimeScriptClass;
public:
#if WITH_EDITORONLY_DATA
	UPROPERTY(Category="Magic Node", EditDefaultsOnly)
	FLinearColor NodeColor;
	//
	UPROPERTY(Category="Magic Node", EditDefaultsOnly)
	TSubclassOf<UMagicNode>ParentClass;
	//
	UPROPERTY(Category="Magic Node", EditDefaultsOnly)
	bool LockSourceCode;
	//
	UPROPERTY(Category="Source Code", EditDefaultsOnly)
	FMGC_SourceCode Source;
#endif
	void PostInitProperties() override;
	//
	const bool RefreshRuntimeScriptClass();
	UClass* GetRuntimeScriptClass() const {return RuntimeScriptClass;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
static T* MGC_Allocate(UObject* Context, UClass* Class) {
	if (Context && Context->GetWorld()) {
		UMagicNode* NewMGC = NewObject<UMagicNode>(Context,Class,Class->GetFName(),RF_Transient);
		NewMGC->RegisterWorld(Context->GetWorld());
		return CastChecked<T>(NewMGC);
	}///
	//
	return nullptr;
}///

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////