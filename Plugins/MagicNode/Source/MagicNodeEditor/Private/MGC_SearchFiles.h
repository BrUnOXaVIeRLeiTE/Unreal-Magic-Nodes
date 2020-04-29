//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MagicNodeLogger.h"
#include "MagicNodeEditor_Shared.h"

#include "MGC_SourceTreeView.h"

#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Async/AsyncWork.h"
#include "Runtime/Core/Public/Async/TaskGraphInterfaces.h"

#include "Runtime/Slate/Public/Widgets/Layout/SBox.h"
#include "Runtime/Slate/Public/Widgets/Input/SButton.h"
#include "Runtime/Slate/Public/Widgets/Layout/SBorder.h"
#include "Runtime/Slate/Public/Widgets/Views/SListView.h"
#include "Runtime/Slate/Public/Widgets/Input/SCheckBox.h"
#include "Runtime/Slate/Public/Widgets/Input/SComboBox.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBar.h"
#include "Runtime/Slate/Public/Widgets/Layout/SScrollBox.h"
#include "Runtime/SlateCore/Public/Widgets/Images/SImage.h"
#include "Runtime/Slate/Public/Widgets/Layout/SSeparator.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableText.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableTextBox.h"
#include "Runtime/Slate/Public/Widgets/Notifications/SProgressBar.h"

#include "Editor/EditorStyle/Public/EditorStyle.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class ESearchState : uint8 {
	COMPLETE,
	SEARCHING
};

struct FSearchInfo {
	FString MatchLine;
	FString MatchFilePath;
	FTextLocation MatchWordLocation;
	//
	//
	bool operator == (const FSearchInfo &Other) const {
		return (
			(MatchLine==Other.MatchLine) &&
			(MatchFilePath==Other.MatchFilePath) &&
			(MatchWordLocation==Other.MatchWordLocation)
		);///
	}///
	//
	bool operator != (const FSearchInfo &Other) const {
		return !(*this==Other);
	}///
	//
	//
	FSearchInfo() {
		MatchLine = FString();
		MatchFilePath = FString();
		MatchWordLocation = FTextLocation();
	}///
	//
	FSearchInfo(const FString &Line, const FString &Path, const FTextLocation &Local) {
		MatchLine = Line;
		MatchFilePath = Path;
		MatchWordLocation = Local;
	}///
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static bool RequestListRefresh = false;
static ESearchState SearchState = ESearchState::COMPLETE;
static TArray<TSharedPtr<FSearchInfo>>SearchResultList = TArray<TSharedPtr<FSearchInfo>>();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TASK_SearchFiles : public FNonAbandonableTask {
public:
	static FString SearchText;
	static FString SearchRoot;
	static bool CaseSensitive;
public:
	static bool IsSourceFile(const FString &Path);
	static bool SearchInFile(const FString &FilePath);
public:
	TASK_SearchFiles(){}
	//
	FORCEINLINE TStatId GetStatId() const {
		RETURN_QUICK_DECLARE_CYCLE_STAT(TASK_SearchFiles,STATGROUP_ThreadPoolAsyncTasks);
	}///
	//
	void DoWork() {
		SearchResultList.Empty();
		//
		for (const TSharedPtr<FSourceTreeNode>&Node : GlobalSourceTreeView) {
			if ((!TASK_SearchFiles::SearchRoot.IsEmpty())&&(TASK_SearchFiles::SearchRoot!=TEXT("All"))) {
				if (!Node->Path.Contains(TASK_SearchFiles::SearchRoot)) {continue;}
			}///
			//
			SearchInFile(Node->FullPath);
			//
			for (const TSharedPtr<FSourceTreeNode>&N1 : Node->ChildNodes) {
				SearchInFile(N1->FullPath);
				for (const TSharedPtr<FSourceTreeNode>&N2 : N1->ChildNodes) {
					SearchInFile(N2->FullPath);
					for (const TSharedPtr<FSourceTreeNode>&N3 : N2->ChildNodes) {
						SearchInFile(N3->FullPath);
						for (const TSharedPtr<FSourceTreeNode>&N4 : N3->ChildNodes) {
							SearchInFile(N4->FullPath);
							for (const TSharedPtr<FSourceTreeNode>&N5 : N4->ChildNodes) {
								SearchInFile(N5->FullPath);
								for (const TSharedPtr<FSourceTreeNode>&N6 : N5->ChildNodes) {
									SearchInFile(N6->FullPath);
			}	}	}	}	}	}///
		}///
		//
		if (SearchResultList.Num()>0) {
			SearchResultList.Sort(
				[](const TSharedPtr<FSearchInfo>&SI, const TSharedPtr<FSearchInfo>&TI)
				{
					if (TI->MatchLine.StartsWith(TEXT("*"))) {return false;}
					if (TI->MatchLine.StartsWith(TEXT("@"))) {return false;}
					if (TI->MatchLine.StartsWith(TEXT("#"))) {return false;}
					if (TI->MatchLine.StartsWith(TEXT("/*"))) {return false;}
					if (TI->MatchLine.StartsWith(TEXT("//"))) {return false;}
					//
					return (TI->MatchLine > SI->MatchLine);
				}///
			);///
			//
			RequestListRefresh = true;
		}///
	}///
	//
	bool CanAbandon() {return true;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SMGC_SearchFiles : public SCompoundWidget {
private:
	static FAsyncTask<TASK_SearchFiles>*ASYNC_Search;
private:
	TSharedPtr<SListView<TSharedPtr<FSearchInfo>>>SEARCH_RESULTS;
	TSharedPtr<SComboBox<TSharedPtr<FString>>>SEARCH_ROOTPATH;
	TSharedPtr<SEditableTextBox>SEARCH_TEXT;
	//
	TSharedPtr<SScrollBox>SEARCH_SCROLLBOX;
	TSharedPtr<SScrollBar>SEARCH_SCROLL;
private:
	FString SearchText;
	FString SelectedSearchRoot;
	TArray<TSharedPtr<FString>>SearchRoots;
	//
	float SearchLoad;
	bool SensitiveSearch;
public:
	SMGC_SearchFiles();
	~SMGC_SearchFiles();
	//
	SLATE_BEGIN_ARGS(SMGC_SearchFiles)
	{}///
		SLATE_ARGUMENT(FString,SearchWord)
	SLATE_END_ARGS()
	//
	//
	void Construct(const FArguments &InArgs);
	void Tick(const FGeometry &AllottedGeometry, const double CurrentTime, const float DeltaTime) override;
	//
	//
	TSharedRef<SWidget>OnGenerateRootSelectionWidget(TSharedPtr<FString>Item);
	TSharedRef<ITableRow>OnGenerateSearchResult(TSharedPtr<FSearchInfo>Item, const TSharedRef<STableViewBase>&OwnerTable);
	//
	void OnClickedSearchResult(TSharedPtr<FSearchInfo>Item);
	void OnSelectedRootChanged(TSharedPtr<FString>Item, ESelectInfo::Type SelectInfo);
	void OnSelectedSearchResult(TSharedPtr<FSearchInfo>Item, ESelectInfo::Type SelectInfo);
	//
	void OnSearchResultScroll(float Offset);
	void OnInternalSearchResultScroll(float Offset);
	void OnSearchSensitiveChanged(ECheckBoxState NewState);
	void OnSearchTextChanged(const FText &InText, ETextCommit::Type CommitType);
	void OnSearchTextComitted(const FText &NewText, ETextCommit::Type CommitInfo);
public:
	bool CanEdit() const;
	bool CanSearch() const;
	bool CanCancel() const;
	TOptional<float>GetSearchLoad() const;
	//
	FText GetSearchText() const;
	FText GetSelectedSearchRoot() const;
	//
	ECheckBoxState IsSearchSensitive() const;
	//
	FReply OnClickedSearchFiles();
	FReply OnClickedCancelSearch();
	//
	EVisibility GetSearchProgressVisibility() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////