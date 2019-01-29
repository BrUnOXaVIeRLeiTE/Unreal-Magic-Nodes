//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeRuntime_Shared.h"
#include "MagicNodeSettings.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UMagicNodeScript;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MGC Global Settings

UCLASS(classGroup=Synaptech, Category="Synaptech", config="MagicNode")
class MAGICNODERUNTIME_API UMGC_Settings : public UObject {
	GENERATED_BODY()
	//
	UMGC_Settings();
public:
	/// :: RUNTIME ::
	///
	UPROPERTY(Category="Runtime Settings", config, EditDefaultsOnly, BlueprintReadOnly)
	bool SomeOption;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////