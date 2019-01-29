//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2019 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MGC_CodeEditorStyle.h"
#include "MagicNodeEditor_Shared.h"

#include "Fonts/CompositeFont.h"

#include "Styling/CoreStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateStyleRegistry.h"

#include "Interfaces/IPluginManager.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MTF_FONT(Name,RelativePath,...) FCompositeFont(Name,FMGC_CodeEditorStyle::InContent(RelativePath,TEXT(".ttf")),EFontHinting::Default,EFontLoadingPolicy::LazyLoad);
#define IMAGE_BRUSH(RelativePath,...) FSlateImageBrush(FMGC_CodeEditorStyle::InContent(RelativePath,TEXT(".png")),__VA_ARGS__)
#define BOX_BRUSH(RelativePath,...) FSlateBoxBrush(FMGC_CodeEditorStyle::InContent(RelativePath,TEXT(".png") ),__VA_ARGS__)
#define TTF_FONT(RelativePath,...) FSlateFontInfo(FMGC_CodeEditorStyle::InContent(RelativePath,TEXT(".ttf")),__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedPtr<FSlateStyleSet>FMGC_CodeEditorStyle::StyleSet = nullptr;
TSharedPtr<ISlateStyle>FMGC_CodeEditorStyle::Get() {return StyleSet;}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FMGC_CodeEditorStyle::InContent(const FString &RelativePath, const TCHAR* Extension) {
	static FString Content = IPluginManager::Get().FindPlugin(TEXT("MagicNode"))->GetContentDir();
	return (Content/RelativePath)+Extension;
}

FName FMGC_CodeEditorStyle::GetStyleSetName() {
	static FName StyleName(TEXT("MGC_NodeStyle"));
	return StyleName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FMGC_CodeEditorStyle::Initialize() {
	if (StyleSet.IsValid()) {return;}
	//
	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(IPluginManager::Get().FindPlugin(TEXT("MagicNode"))->GetContentDir());
	//
	//
	const FVector2D Icon16x16(16.f,16.f);
	const FVector2D Icon22x22(22.f,22.f);
	const FVector2D Icon32x32(32.f,32.f);
	const FVector2D Icon40x40(40.f,40.f);
	const FVector2D Icon64x64(64.f,64.f);
	const FVector2D Icon128x128(128.f,128.f);
	//
	FSlateFontInfo TTF_Source = TTF_FONT(TEXT("UI/SourceCodePro"),12);
	FSlateFontInfo TTF_Hack = TTF_FONT(TEXT("UI/Hack"),14);
	//
	const FTextBlockStyle TBS_CodeStyle = FTextBlockStyle()
	.SetHighlightShape(BOX_BRUSH("UI/KMGC_TextHighlight",FMargin(3.f/8.f)))
	.SetHighlightColor(FLinearColor(FColor(255,175,5,255)))
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetColorAndOpacity(FLinearColor::White)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetFont(TTF_Hack);
	//
	const FTextBlockStyle TBS_SourceStyle = FTextBlockStyle()
	.SetHighlightShape(BOX_BRUSH("UI/KMGC_TextHighlight",FMargin(3.f/8.f)))
	.SetHighlightColor(FLinearColor(FColor(255,175,5,255)))
	.SetShadowColorAndOpacity(FLinearColor::Black)
	.SetColorAndOpacity(FLinearColor::White)
	.SetShadowOffset(FVector2D::ZeroVector)
	.SetFont(TTF_Source);
	//
	//
	/// Icons:
	{
		StyleSet->Set("MGC.MagicNodeIcon.Small", new IMAGE_BRUSH(TEXT("Icons/MagicNode_128x"),Icon16x16));
		StyleSet->Set("MGC.MagicNodeIcon", new IMAGE_BRUSH(TEXT("Icons/MagicNode_128x"),Icon128x128));
		//
		StyleSet->Set("MGC.Toolbar.H", new IMAGE_BRUSH(TEXT("UI/KMGC_H_22x"),Icon22x22));
		StyleSet->Set("MGC.Toolbar.T", new IMAGE_BRUSH(TEXT("UI/KMGC_T_22x"),Icon22x22));
		StyleSet->Set("MGC.Toolbar.CPP", new IMAGE_BRUSH(TEXT("UI/KMGC_CPP_22x"),Icon22x22));
		StyleSet->Set("MGC.Toolbar.SaveScript", new IMAGE_BRUSH(TEXT("UI/KMGC_Save_40x"),Icon40x40));
		StyleSet->Set("MGC.Toolbar.MagicNode", new IMAGE_BRUSH(TEXT("Icons/MagicNode_128x"),Icon40x40));
		StyleSet->Set("MGC.Toolbar.ReloadScript", new IMAGE_BRUSH(TEXT("UI/KMGC_Reload_40x"),Icon40x40));
	}
	//
	/// Text Editor:
	{
		StyleSet->Set("MGC.CodeBlockStyle",TBS_CodeStyle);
		StyleSet->Set("MGC.SourceBlockStyle",TBS_SourceStyle);
		//
		StyleSet->Set("MGC.SyntaxHighlight.Processor",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(115,115,115,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Operator",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(95,255,245,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Normal",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,235,195,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Keyword",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(50,125,255,255))));
		StyleSet->Set("MGC.SyntaxHighlight.String",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,155,135,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Function",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,105,5,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Number",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(55,255,155,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Comment",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(55,125,65,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Class",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(75,185,245,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Macro",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(225,10,155,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Illegal",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(175,5,5,255))));
		StyleSet->Set("MGC.SyntaxHighlight.Type",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,175,5,255))));
	}
	//
	//
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

void FMGC_CodeEditorStyle::Shutdown() {
	if (StyleSet.IsValid()) {
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique()); StyleSet.Reset();
	}///
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#undef TTF_FONT
#undef MTF_FONT
#undef BOX_BRUSH
#undef IMAGE_BRUSH

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////