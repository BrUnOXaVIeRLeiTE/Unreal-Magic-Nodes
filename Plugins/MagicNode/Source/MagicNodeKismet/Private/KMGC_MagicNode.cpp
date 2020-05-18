//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_MagicNode.h"

#include "UObject/UnrealType.h"
#include "Engine/EngineTypes.h"
#include "EdGraph/EdGraph.h"

#include "Editor.h"
#include "KismetCompiler.h"
#include "EdGraphSchema_K2.h"
#include "KismetCompilerMisc.h"
#include "EditorCategoryUtils.h"
#include "K2Node_CallFunction.h"
#include "BlueprintNodeSpawner.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "BlueprintActionDatabaseRegistrar.h"

#include "Editor/UnrealEd/Public/SourceCodeNavigation.h"
#include "Editor/LevelEditor/Public/LevelEditorActions.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct UKMGC_MagicNodeHelper {
	static FName PN_Script;
	static FName PN_Context;
	//
	static FName EXE;
};

FName UKMGC_MagicNodeHelper::EXE(TEXT("Execute"));
FName UKMGC_MagicNodeHelper::PN_Script(TEXT("Script"));
FName UKMGC_MagicNodeHelper::PN_Context(TEXT("Context"));

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "Synaptech"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UKMGC_MagicNode::UKMGC_MagicNode(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
	NodeTooltip = LOCTEXT("MGC_NodeTooltip","Magic Node (C++)");
	//
	KMGC_InitParser();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FText UKMGC_MagicNode::GetNodeTitle(ENodeTitleType::Type TType) const {
	FText Title = NSLOCTEXT("K2Node","KMGC_MagicNode_BaseTitle","Magic Node (C++)");
	//
	if (TType != ENodeTitleType::MenuTitle) {
		if (GetScriptObject()==nullptr) {return Title;}
		//
		
		FString SName = GetScriptObject()->GetName();
		FString SCaps; SCaps.AppendChar(SName[0]);
		//
		SName.RemoveFromStart(TEXT("Default__"));
		SName.RemoveFromEnd(TEXT("_C"));
		SName.ReplaceInline(TEXT("_"),TEXT(" "));
		//
		for (int32 I=1; I < SName.Len(); I++) {
			if (FChar::IsUpper(SName[I])) {
                if (SName[I-1] != TEXT(' ') && !FChar::IsUpper(SName[I-1])) {SCaps.AppendChar(TEXT(' '));}
			} SCaps.AppendChar(SName[I]);
        }///
		//
		FText Name = FText::FromString(SCaps);
		FFormatNamedArguments Args; Args.Add(TEXT("Name"),Name);
		//
		CachedNodeTitle.SetCachedText(FText::Format(NSLOCTEXT("K2Node","KMGC_MagicNode_Title","(C++) :: {Name}"),Args),this);
		Title = CachedNodeTitle;
	}///
	//
	return Title;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UKMGC_MagicNode::AllocateDefaultPins() {
	Super::AllocateDefaultPins();
	//
	//
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	ErrorType = EMessageSeverity::Info;
	//
	//
	/// Pin Type Definitions.
	FEdGraphPinType PNT_Exec(KSchema->PC_Exec,TEXT(""),nullptr,EPinContainerType::None,false,FEdGraphTerminalType());
	FEdGraphPinType PNT_Object(KSchema->PC_Object,TEXT(""),UObject::StaticClass(),EPinContainerType::None,false,FEdGraphTerminalType());
	FEdGraphPinType PNT_Script(KSchema->PC_Object,TEXT(""),UMagicNodeScript::StaticClass(),EPinContainerType::None,false,FEdGraphTerminalType());
	//
	//
	/// Node's Execution Pins.
	UEdGraphPin* PIN_Finish = GetThenPin();
	UEdGraphPin* PIN_Execute = GetExecPin();
	if (PIN_Finish==nullptr) {PIN_Finish=CreatePin(EGPD_Output,PNT_Exec,KSchema->PN_Then);}
	if (PIN_Execute==nullptr) {PIN_Execute=CreatePin(EGPD_Input,PNT_Exec,KSchema->PN_Execute);}
	//
	/// Node's Context Pin.
	UEdGraphPin* PIN_Context = GetContextPin();
	if (PIN_Context==nullptr) {PIN_Context=CreatePin(EGPD_Input,PNT_Object,UKMGC_MagicNodeHelper::PN_Context);}
	//
	/// Node's Script Pin.
	UEdGraphPin* PIN_Script = GetScriptPin();
	if (PIN_Script==nullptr) {PIN_Script=CreatePin(EGPD_Input,PNT_Script,UKMGC_MagicNodeHelper::PN_Script);}
	//
	//
	PIN_Script->bHidden = true;
	PIN_Script->bAdvancedView = true;
	//
	//
	RuntimeScriptObject.Reset();
	//
	const UMagicNodeScript* ScriptObject = GetScriptObject();
	UFunction* Function = ExpandScript(ScriptObject);
	CreatePinsForScript(Function);
	//
	//
	if (GetBlueprint()==nullptr) {return;}
	if (!GetBlueprint()->ParentClass->HasMetaDataHierarchical(FBlueprintMetadata::MD_ShowWorldContextPin)) {PIN_Context->bHidden=true;}
	//
	//
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UKMGC_MagicNode::ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph* SourceGraph) {
	if (this->IsPendingKill()||!SourceGraph->IsValidLowLevelFast()||SourceGraph->IsPendingKill()) {BreakAllNodeLinks(); return;}
	Super::ExpandNode(CompilerContext,SourceGraph);
	//
	//
	UKMGC_MagicNode* Self = this;
	const UMagicNodeScript* ScriptObject = GetScriptObject();
	//
	UEdGraphPin* PIN_Exec = Self->GetExecPin();
	UEdGraphPin* PIN_Then = Self->GetThenPin();
	UEdGraphPin* PIN_Script = Self->GetScriptPin();
	UEdGraphPin* PIN_Context = Self->FindPinChecked(UKMGC_MagicNodeHelper::PN_Context);
	//
	if (ScriptObject==nullptr) {
		CompilerContext.MessageLog.Error(*LOCTEXT("KMGC_MagicNode_Script_Error","[Magic Node]: @@ must have a @@ specified!").ToString(),Self,PIN_Script);
		Self->BreakAllNodeLinks();
	return;}
	//
	//
	UFunction* Function = ExpandScript(ScriptObject);
	//
	if (Function==nullptr) {
		CompilerContext.MessageLog.Error(*LOCTEXT("KMGC_MagicNode_Function_Error", "[Magic Node]: @@ unable to allocate Executable Function; Consider compiling or hot-reloading the C++ project before binding this node.").ToString(),Self);
	} else {
		UK2Node_CallFunction* MGC_Execute = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(Self,SourceGraph);
		MGC_Execute->SetFromFunction(Function);
		MGC_Execute->AllocateDefaultPins();
		//
		UEdGraphPin* PUT_Context = MGC_Execute->FindPinChecked(UKMGC_MagicNodeHelper::PN_Context);
		UEdGraphPin* PUT_Exec = MGC_Execute->GetExecPin();
		UEdGraphPin* PUT_Then = MGC_Execute->GetThenPin();
		//
		check(PIN_Script);
		check(PIN_Exec); check(PUT_Exec);
		check(PIN_Then); check(PUT_Then);
		check(PIN_Context); check(PUT_Context);
		//
		CompilerContext.CopyPinLinksToIntermediate(*PIN_Context,*PUT_Context);
		CompilerContext.MovePinLinksToIntermediate(*PIN_Exec,*PUT_Exec);
		CompilerContext.MovePinLinksToIntermediate(*PIN_Then,*PUT_Then);
		//
		for (int32 I=0; I<Pins.Num(); I++) {
			UEdGraphPin* PIN = Pins[I];
			//
			if (IsDefaultPin(PIN)) {continue;}
			//
			UEdGraphPin* PUT = MGC_Execute->FindPin(PIN->GetFName());
			if (PUT) {CompilerContext.MovePinLinksToIntermediate(*PIN,*PUT);}
		}///
	}///
	//
	//
	Self->BreakAllNodeLinks();
}

UFunction* UKMGC_MagicNode::ExpandScript(const UMagicNodeScript* Script) {
	if (Script==nullptr) {return nullptr;}
	//
	UMagicNode* RuntimeScript = GetRuntimeScriptObject();
	if (RuntimeScript==nullptr) {return nullptr;}
	//
	return RuntimeScript->FindFunction(UKMGC_MagicNodeHelper::EXE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UKMGC_MagicNode::PinConnectionListChanged(UEdGraphPin* ChangedPin) {
	if (ChangedPin && (ChangedPin->PinName==UKMGC_MagicNodeHelper::PN_Script)) {ScriptPinChanged();}
}

void UKMGC_MagicNode::PinDefaultValueChanged(UEdGraphPin* ChangedPin) {
	if (ChangedPin && (ChangedPin->PinName==UKMGC_MagicNodeHelper::PN_Script)) {ScriptPinChanged();}
}

void UKMGC_MagicNode::ScriptPinChanged() {
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	//
	const UMagicNodeScript* ScriptObject = GetScriptObject();
	if (ScriptObject==nullptr) {return;}
	//
	for (int32 I=0; I<Pins.Num(); I++) {
		UEdGraphPin* Pin = Pins[I];
		if (!IsDefaultPin(Pin)){Pin->BreakAllPinLinks(true);}
	}///
	//
	AllocateDefaultPins();
	//
	UFunction* Function = ExpandScript(ScriptObject);
	CreatePinsForScript(Function);
	//
	UEdGraph* Graph = GetGraph();
	Graph->NotifyGraphChanged();
	CachedNodeTitle.MarkDirty();
	NodeTooltip = FText::FromString(ScriptObject->Source.Script);
	//
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UKMGC_MagicNode::CreatePinsForScript(UFunction* Function) {
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	//
	//
	TArray<UEdGraphPin*>OldPins=Pins;
	TArray<UEdGraphPin*>OldClassPins;
	TArray<UEdGraphPin*>NewClassPins;
	//
	for (int32 I=0; I<OldPins.Num(); I++) {
		UEdGraphPin* OldPin = OldPins[I];
		if (!IsDefaultPin(OldPin)) {
			OldClassPins.Add(OldPin);
			Pins.Remove(OldPin);
		}///
	}///
	//
	//
	if (Function!=nullptr) {
	for (TFieldIterator<FProperty>PIT(Function); PIT&&(PIT->PropertyFlags & CPF_Parm); ++PIT) {
		if (IsDefaultParam(*PIT)) {continue;}
		//
		TFieldPath<FProperty>Property = *PIT;
		if (PIT->HasAnyPropertyFlags(CPF_OutParm|CPF_ReturnParm)&&(!PIT->HasAnyPropertyFlags(CPF_ReferenceParm))) {
			if (FindPin(PIT->GetFName())==nullptr && FBlueprintEditorUtils::PropertyStillExists(*PIT)) {
				if (UEdGraphPin* Pin=CreatePin(EGPD_Output,NAME_None,Property->GetFName())) {
					KSchema->ConvertPropertyToPinType(Property.Get(),Pin->PinType);
					NewClassPins.Add(Pin);
					//
					FString DefaultValue;
					if (KSchema->FindFunctionParameterDefaultValue(Function,Property.Get(),DefaultValue)) {
						KSchema->SetPinAutogeneratedDefaultValue(Pin,DefaultValue);
					} else {KSchema->SetPinAutogeneratedDefaultValueBasedOnType(Pin);}
					//
					KSchema->ConstructBasicPinTooltip(*Pin,Property->GetToolTipText(),Pin->PinToolTip);
				}///
			}///
		} else {
			if (FindPin(PIT->GetFName())==nullptr && FBlueprintEditorUtils::PropertyStillExists(*PIT)) {
				if (UEdGraphPin* Pin=CreatePin(EGPD_Input,NAME_None,Property->GetFName())) {
					KSchema->ConvertPropertyToPinType(Property.Get(),Pin->PinType);
					NewClassPins.Add(Pin);
					//
					FString DefaultValue;
					if (KSchema->FindFunctionParameterDefaultValue(Function,Property.Get(),DefaultValue)) {
						KSchema->SetPinAutogeneratedDefaultValue(Pin,DefaultValue);
					} else {KSchema->SetPinAutogeneratedDefaultValueBasedOnType(Pin);}
					//
					KSchema->ConstructBasicPinTooltip(*Pin,Property->GetToolTipText(),Pin->PinToolTip);
				}///
			}///
		}///
	}}///
	//
	//
	TMap<UEdGraphPin*,UEdGraphPin*>NewPins2OldPins;
	RewireOldPinsToNewPins(OldClassPins,NewClassPins,&NewPins2OldPins);
	//
	DestroyPinList(OldClassPins);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FNodeHandlingFunctor* UKMGC_MagicNode::CreateNodeHandler(FKismetCompilerContext &CompilerContext) const {
	return new FNodeHandlingFunctor(CompilerContext);
}

void UKMGC_MagicNode::PostPlacedNewNode() {
	Super::PostPlacedNewNode();
	//
	const UMagicNodeScript* ScriptObject = GetScriptObject();
	UFunction* Function = ExpandScript(ScriptObject);
	CreatePinsForScript(Function);
}

void UKMGC_MagicNode::PostReconstructNode() {
	Super::PostReconstructNode();
	//
	const UMagicNodeScript* ScriptObject = GetScriptObject();
	UFunction* Function = ExpandScript(ScriptObject);
	CreatePinsForScript(Function);
	//
	if (ScriptObject != nullptr) {
		NodeTooltip=FText::FromString(ScriptObject->Source.Script);
	}///
}

void UKMGC_MagicNode::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>&OldPins) {
	AllocateDefaultPins();
	//
	const UMagicNodeScript* ScriptObject = GetScriptObject();
	UFunction* Function = ExpandScript(ScriptObject);
	CreatePinsForScript(Function);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UEdGraphPin* UKMGC_MagicNode::GetExecPin() const {
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	//
	UEdGraphPin* Pin = FindPin(KSchema->PN_Execute,EGPD_Input);
	check(Pin==nullptr||Pin->Direction==EGPD_Input); return Pin;
}

UEdGraphPin* UKMGC_MagicNode::GetThenPin() const {
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	//
	UEdGraphPin* Pin = FindPin(KSchema->PN_Then,EGPD_Output);
	check(Pin==nullptr||Pin->Direction==EGPD_Output); return Pin;
}

UEdGraphPin* UKMGC_MagicNode::GetScriptPin(const TArray<UEdGraphPin*>* InPinsToSearch) const {
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;
	//
	UEdGraphPin* Pin = nullptr;
	for (auto IT = PinsToSearch->CreateConstIterator(); IT; ++IT) {
		UEdGraphPin* Source = *IT;
		if (Source && (Source->PinName==UKMGC_MagicNodeHelper::PN_Script)) {Pin=Source; break;}
	} check(Pin==nullptr||Pin->Direction==EGPD_Input);
	//
	return Pin;
}

UEdGraphPin* UKMGC_MagicNode::GetContextPin(const TArray<UEdGraphPin*>* InPinsToSearch) const {
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;
	//
	UEdGraphPin* Pin = nullptr;
	for (auto IT = PinsToSearch->CreateConstIterator(); IT; ++IT) {
		UEdGraphPin* Source = *IT;
		if (Source && (Source->PinName==UKMGC_MagicNodeHelper::PN_Context)) {Pin=Source; break;}
	} check(Pin==nullptr||Pin->Direction==EGPD_Input);
	//
	return Pin;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool UKMGC_MagicNode::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const  {
	UBlueprint* Blueprint = FBlueprintEditorUtils::FindBlueprintForGraph(TargetGraph);
	return Super::IsCompatibleWithGraph(TargetGraph) && (!Blueprint||(FBlueprintEditorUtils::FindUserConstructionScript(Blueprint)!= TargetGraph && Blueprint->GeneratedClass->GetDefaultObject()->ImplementsGetWorld()));
}

bool UKMGC_MagicNode::IsDefaultPin(UEdGraphPin* Pin) {
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	//
	return (
		Pin->PinName == KSchema->PN_Then ||
		Pin->PinName == KSchema->PN_Execute ||
		Pin->PinName == UKMGC_MagicNodeHelper::PN_Script ||
		Pin->PinName == UKMGC_MagicNodeHelper::PN_Context
	);//
}

bool UKMGC_MagicNode::IsDefaultParam(const TFieldPath<FProperty>Param) {
	const UEdGraphSchema_K2* KSchema = GetDefault<UEdGraphSchema_K2>();
	//
	return (
		Param->GetFName()==KSchema->PN_Then ||
		Param->GetFName()==KSchema->PN_Execute ||
		Param->GetFName()==UKMGC_MagicNodeHelper::PN_Script ||
		Param->GetFName()==UKMGC_MagicNodeHelper::PN_Context
	);//
}

bool UKMGC_MagicNode::HasScript() const {
	return (GetScriptObject()!=nullptr);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FName UKMGC_MagicNode::GetCornerIcon() const {
	return TEXT("KMGC.MagicNodeIcon");
}

FText UKMGC_MagicNode::GetTooltipText() const {
	return NodeTooltip;
}

FLinearColor UKMGC_MagicNode::GetNodeTitleColor() const {
	if (!HasScript()) {return FLinearColor(FColor(5,15,25));}
	//
	UMagicNodeScript* Script = GetScriptObject();
	//
	return Script->NodeColor;
}

FSlateIcon UKMGC_MagicNode::GetIconAndTint(FLinearColor &OutColor) const {
	static FSlateIcon Icon(TEXT("MagicNodeEditorStyle"),TEXT("ClassIcon.MagicNode"));
	//
	return Icon;
}

FText UKMGC_MagicNode::GetMenuCategory() const {
	return LOCTEXT("MagicNodeCategory","Magic Node");
}

void UKMGC_MagicNode::GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const {
	UClass* ActionKey = GetClass();
	//
	if (ActionRegistrar.IsOpenForRegistration(ActionKey)) {
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		if (NodeSpawner==nullptr) {return;}
		//
		ActionRegistrar.AddBlueprintAction(ActionKey,NodeSpawner);
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UMagicNodeScript* UKMGC_MagicNode::GetScriptObject() const {
	const TArray<UEdGraphPin*>* PinsToSearch = &Pins;
	//
	UEdGraphPin* PIN_Script = GetScriptPin(PinsToSearch);
	UMagicNodeScript* ScriptObject = nullptr;
	//
	if (PIN_Script==nullptr) {return nullptr;}
	ScriptObject = Cast<UMagicNodeScript>(PIN_Script->DefaultObject);
	//
	return ScriptObject;
}

UMagicNode* UKMGC_MagicNode::GetRuntimeScriptObject() {
	if (!HasScript()) {return nullptr;}
	//
	if (!RuntimeScriptObject.IsValid()) {
		if (GetScriptObject()->RefreshRuntimeScriptClass()) {
			UMagicNodeScript* Script = GetScriptObject();
			//
			FString Name=Script->GetName();
			Name.RemoveFromEnd(TEXT("_C"));
			Name.RemoveFromStart(TEXT("Default__"));
			//
			if (Script->RefreshRuntimeScriptClass()) {
				UClass* Class = Script->GetRuntimeScriptClass();
				RuntimeScriptObject = NewObject<UMagicNode>(GetTransientPackage(),Class,FName(*Name),RF_Transient);
			}///
		}///
	} return RuntimeScriptObject.Get();
}

FString UKMGC_MagicNode::GetScriptText() const {
	UMagicNodeScript* Script = GetScriptObject();
	if (Script==nullptr) {return FString();}
	//
	FString Name = Script->GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	Name.RemoveFromStart(TEXT("Default__"));
	//
	Script->Source.Script.ReplaceInline(TEXT("{ScriptClass}"),*Name);
	Script->Source.Script.ReplaceInline(TEXT("{ScriptName}"),*Name);
	//
	return Script->Source.Script;
}

FString UKMGC_MagicNode::GetHeaderText() const {
	UMagicNodeScript* Script = GetScriptObject();
	if (Script==nullptr) {return FString();}
	//
	FString Name = Script->GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	Name.RemoveFromStart(TEXT("Default__"));
	//
	Script->Source.Header.ReplaceInline(TEXT("{ScriptClass}"),*Name);
	Script->Source.Header.ReplaceInline(TEXT("{ScriptName}"),*Name);
	//
	return Script->Source.Header;
}

FString UKMGC_MagicNode::GetTypesText() const {
	UMagicNodeScript* Script = GetScriptObject();
	if (Script==nullptr) {return FString();}
	//
	FString Name = Script->GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	Name.RemoveFromStart(TEXT("Default__"));
	//
	Script->Source.Header.ReplaceInline(TEXT("{ScriptClass}"),*Name);
	Script->Source.Header.ReplaceInline(TEXT("{ScriptName}"),*Name);
	//
	return Script->Source.Types;
}

FString UKMGC_MagicNode::GetParentClass() const {
	if (GetScriptObject()==nullptr) {return FString();}
	UMagicNodeScript* Script = GetScriptObject();
	//
	FString Name = Script->ParentClass->GetName();
	Name.RemoveFromEnd(TEXT("_C"));
	//
	return Name;
}

TArray<FString>UKMGC_MagicNode::GetIncludes() const {
	if (GetScriptObject()==nullptr) {return TArray<FString>();}
	//
	UMagicNodeScript* Script = GetScriptObject();
	//
	return Script->Source.Includes;
}

TArray<FString>UKMGC_MagicNode::GetMacros() const {
	if (GetScriptObject()==nullptr) {return TArray<FString>();}
	//
	UMagicNodeScript* Script = GetScriptObject();
	//
	return Script->Source.Macros;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UKMGC_MagicNode::SetScriptObject(UMagicNodeScript* New) {
	const TArray<UEdGraphPin*>* PinsToSearch = &Pins;
	//
	UEdGraphPin* PIN_Script = GetScriptPin(PinsToSearch);
	if (PIN_Script==nullptr) {return;}
	//
	const bool Changed = !(New==PIN_Script->DefaultObject);
	PIN_Script->DefaultObject = New;
	//
	if (New) {
		FString Code = New->Source.Script;
		NodeTooltip = FText::FromString(Code);
	}///
	//
	if (Changed) {ScriptPinChanged();}
}

void UKMGC_MagicNode::SetTooltip(const FString &New) {
	NodeTooltip = FText::FromString(New);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool UKMGC_MagicNode::CanCompileProject() const {
	return FLevelEditorActionCallbacks::Recompile_CanExecute();
}

void UKMGC_MagicNode::CompileProject() {
	if (FSourceCodeNavigation::IsCompilerAvailable()) {
		FLevelEditorActionCallbacks::RecompileGameCode_Clicked();
	}///
}

void UKMGC_MagicNode::CompileScript() {
	UMagicNodeScript* ScriptObject = GetScriptObject();
	//
	if (GetBlueprint()==nullptr) {
		LOG_MGC(EMGCSeverity::Error,TEXT("{MGC}:: Compiler unreachable, Script is invalid."));	
	return;}
	//
	if (!ScriptObject->IsValidLowLevel()) {
		ErrorMsg = TEXT("{MGC}:: Compiler unreachable, Script is invalid.");
		GetBlueprint()->Message_Error(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
		ScriptNodeWidget->SetErrorMessage(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
	return;} else if (ScriptObject==UMagicNodeScript::StaticClass()->ClassDefaultObject) {
		GetBlueprint()->Message_Error(TEXT("{MGC}:: Compiler unreachable, Script is invalid (referencing base Magic Node Script Class is not allowed)."));
	return;}
	//
	CompilerResult = CompileScriptClass(ScriptObject->GetName(),GetHeaderText(),GetScriptText(),GetTypesText(),GetParentClass(),GetIncludes(),GetMacros());
	if (CompilerResult != EMGC_CompilerResult::None) {
		switch (CompilerResult) {
			case EMGC_CompilerResult::Compiled:
			{
				ErrorType = EMessageSeverity::Info;
				LOG_MGC(EMGCSeverity::Info,Message[(uint32)CompilerResult]);
				ErrorMsg = TEXT(""); ScriptNodeWidget->SetErrorMessage(TEXT(""));
				GetBlueprint()->Message_Note(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
				//
				if (GEditor) {
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
				} CompileProject();
			} break;
			case EMGC_CompilerResult::ParsingFailure:
			{
				ErrorType = EMessageSeverity::Warning;
				LOG_MGC(EMGCSeverity::Warning,Message[(uint32)CompilerResult]);
				ErrorMsg = FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]);
				GetBlueprint()->Message_Warn(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
				ScriptNodeWidget->SetErrorMessage(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
				//
				if (GEditor) {
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
				} CompileProject();
			} break;
			default:
			{
				ErrorType = EMessageSeverity::Error;
				LOG_MGC(EMGCSeverity::Error,Message[(uint32)CompilerResult]);
				ErrorMsg = FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]);
				GetBlueprint()->Message_Error(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
				ScriptNodeWidget->SetErrorMessage(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
				//
				if (GEditor) {
					GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileFailed_Cue.CompileFailed_Cue"));
				}///
			}///
		break;}
	} else {ErrorMsg = TEXT(""); ScriptNodeWidget->SetErrorMessage(TEXT(""));}
	//
	UpdateDatabaseReferences();
}

void UKMGC_MagicNode::ReloadRuntimeScript() {
	UMagicNodeScript* ScriptObject = GetScriptObject();
	//
	if (GetBlueprint()==nullptr) {
		LOG_MGC(EMGCSeverity::Error,TEXT("{MGC}:: Reload unreachable, Script is invalid."));	
	return;}
	//
	if (!ScriptObject->IsValidLowLevel()) {
		ErrorMsg = TEXT("{MGC}:: Reload unreachable, Script is invalid.");
		GetBlueprint()->Message_Error(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
		ScriptNodeWidget->SetErrorMessage(FString::Printf(TEXT("{MGC}:: %s"),*Message[(uint32)CompilerResult]));
	return;} else if (ScriptObject==UMagicNodeScript::StaticClass()->ClassDefaultObject) {
		GetBlueprint()->Message_Error(TEXT("{MGC}:: Reload unreachable, Script is invalid (referencing base Magic Node Script Class is not allowed)."));
	return;}
	//
	if (GetRuntimeScriptObject()!=nullptr) {
		ScriptPinChanged();
		//
		if (GEditor) {
			GEditor->PlayEditorSound(TEXT("/Engine/EditorSounds/Notifications/CompileSuccess_Cue.CompileSuccess_Cue"));
		}///
	}///
	//
	UpdateDatabaseReferences();
}

void UKMGC_MagicNode::UpdateDatabaseReferences() {
	const auto &_Settings = GetDefault<UKMGC_Settings>();
	//
	if (UMGC_SemanticDB::DBState==EDatabaseState::ASYNCLOADING) {return;}
	if (_Settings->SemanticDB.Num()==0) {return;}
	if (!HasScript()) {return;}
	//
	UMGC_SemanticDB* SemanticDB = _Settings->SemanticDB.Array()[0].Get();
	UMagicNodeScript* Script = GetScriptObject();
	//
	if (SemanticDB==nullptr) {return;}
	if (Script==nullptr) {return;}
	//
	if (Script->RefreshRuntimeScriptClass()) {
		SemanticDB->RegisterClassReflection(Script->GetRuntimeScriptClass(),TEXT("U"));
		//
		FClassRedirector CR;
		CR.ObjectName = GetRuntimeScriptObject()->GetName();
		CR.ObjectClass = FString(TEXT("U"))+(Script->GetRuntimeScriptClass()->GetName());
		SemanticDB->ClassRedirectors.Add(CR.ObjectName,CR);
		//
		FString Class = FString::Printf(TEXT("U%s"),*Script->GetRuntimeScriptClass()->GetName());
		IKMGC_ScriptParser::ParseClassFromHeader(GetHeaderText(),Class);
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedPtr<SGraphNode>UKMGC_MagicNode::CreateVisualWidget() {
	return SAssignNew(ScriptNodeWidget,SKMGC_MagicNodeWidget,this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////