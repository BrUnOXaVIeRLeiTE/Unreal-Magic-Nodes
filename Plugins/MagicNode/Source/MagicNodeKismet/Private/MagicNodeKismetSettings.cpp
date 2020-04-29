//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MagicNodeKismetSettings.h"
#include "KMGC_KismetTypes.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

UKMGC_Settings::UKMGC_Settings() {
#if WITH_EDITOR
	ShowKeywordHints = true;
	ScanUnrealTypesOnEditorStartup = false;
	//
	LoadConfig();
	//
	//
	static ConstructorHelpers::FObjectFinder<UMGC_FunctionDB>FunctionCoreDB(TEXT("MGC_FunctionDB'/MagicNode/Database/MGC_Functions.MGC_Functions'"));
	static ConstructorHelpers::FObjectFinder<UMGC_SemanticDB>SemanticCoreDB(TEXT("MGC_SemanticDB'/MagicNode/Database/MGC_Semantics.MGC_Semantics'"));
	static ConstructorHelpers::FObjectFinder<UMGC_KeywordDB>KeywordCoreDB(TEXT("MGC_KeywordDB'/MagicNode/Database/MGC_Keywords.MGC_Keywords'"));
	static ConstructorHelpers::FObjectFinder<UMGC_ClassDB>ClassCoreDB(TEXT("MGC_ClassDB'/MagicNode/Database/MGC_Classes.MGC_Classes'"));
	//
	if (FunctionCoreDB.Succeeded()) {FunctionDB.Add(TSoftObjectPtr<UMGC_FunctionDB>(FunctionCoreDB.Object));}
	if (SemanticCoreDB.Succeeded()) {SemanticDB.Add(TSoftObjectPtr<UMGC_SemanticDB>(SemanticCoreDB.Object));}
	if (KeywordCoreDB.Succeeded()) {KeywordDB.Add(TSoftObjectPtr<UMGC_KeywordDB>(KeywordCoreDB.Object));}
	if (ClassCoreDB.Succeeded()) {ClassDB.Add(TSoftObjectPtr<UMGC_ClassDB>(ClassCoreDB.Object));}
#endif
}

void UKMGC_Settings::PostLoad() {
	Super::PostLoad();
	//
	for (auto DB : FunctionDB) {
		if (DB.IsValid()) {DB.LoadSynchronous();}
	}///
	//
	for (auto DB : KeywordDB) {
		if (DB.IsValid()) {DB.LoadSynchronous();}
	}///
	//
	for (auto DB : ClassDB) {
		if (DB.IsValid()) {DB.LoadSynchronous();}
	}///
	//
	for (auto DB : SemanticDB) {
		if (DB.IsValid()) {DB.LoadSynchronous();}
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////