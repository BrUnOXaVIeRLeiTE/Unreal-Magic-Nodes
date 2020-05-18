//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeRuntime.h"

#include "KMGC_ScriptParser.h"
#include "KMGC_KismetTypes.h"
#include "KMGC_NodeWidget.h"

#include "K2Node.h"
#include "Textures/SlateIcon.h"
#include "UObject/ObjectMacros.h"
#include "EdGraph/EdGraphNodeUtils.h"

#include "KMGC_MagicNode.generated.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UEdGraph;
class FBlueprintActionDatabaseRegistrar;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UCLASS()
class MAGICNODEKISMET_API UKMGC_MagicNode : public UK2Node, public IKMGC_ScriptParser {
	GENERATED_UCLASS_BODY()
protected:
	TWeakObjectPtr<UMagicNode>RuntimeScriptObject = nullptr;
	//
	TSharedPtr<SKMGC_MagicNodeWidget>ScriptNodeWidget;
	EMGC_CompilerResult CompilerResult;
	FNodeTextCache CachedNodeTitle;
	FText NodeTooltip;
	//
	void ScriptPinChanged();
protected:
	void CompileProject();
public:
	virtual void ExpandNode(FKismetCompilerContext &CompilerContext, UEdGraph* SourceGraph) override;
	virtual UFunction* ExpandScript(const UMagicNodeScript* Script);
	//
	//
	virtual void PostPlacedNewNode() override;
	virtual void PostReconstructNode() override;
	virtual void AllocateDefaultPins() override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual void PinConnectionListChanged(UEdGraphPin* Pin) override;
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>&OldPins) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar &ActionRegistrar) const override;
	//
	virtual bool IsNodeSafeToIgnore() const override {return false;}
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	virtual class FNodeHandlingFunctor* CreateNodeHandler(FKismetCompilerContext &CompilerContext) const override;
	//
	virtual FName GetCornerIcon() const override;
	virtual FText GetTooltipText() const override;
	virtual FText GetMenuCategory() const override;
	virtual FLinearColor GetNodeTitleColor() const override;
	virtual bool ShowPaletteIconOnNode() const override {return true;}
	virtual FText GetNodeTitle(ENodeTitleType::Type TType) const override;
	virtual FSlateIcon GetIconAndTint(FLinearColor &OutColor) const override;
	//
	virtual TSharedPtr<SGraphNode>CreateVisualWidget() override;
	//
	//
	bool HasScript() const;
	bool CanCompileProject() const;
	bool IsDefaultPin(UEdGraphPin* Pin);
	bool IsDefaultParam(const TFieldPath<FProperty>Param);
	void CreatePinsForScript(UFunction* Function=nullptr);
	//
	UEdGraphPin* GetExecPin() const;
	UEdGraphPin* GetThenPin() const;
	UEdGraphPin* GetScriptPin(const TArray<UEdGraphPin*>*InPinsToSearch=nullptr) const;
	UEdGraphPin* GetContextPin(const TArray<UEdGraphPin*>*InPinsToSearch=nullptr) const;
	//
	//
	FString GetTypesText() const;
	FString GetScriptText() const;
	FString GetHeaderText() const;
	FString GetParentClass() const;
	TArray<FString>GetMacros() const;
	TArray<FString>GetIncludes() const;
	//
	UMagicNode* GetRuntimeScriptObject();
	UMagicNodeScript* GetScriptObject() const;
	//
	void SetTooltip(const FString &New);
	void SetScriptObject(UMagicNodeScript* New);
	//
	void CompileScript();
	void ReloadRuntimeScript();
	//
	//
	void UpdateDatabaseReferences();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////