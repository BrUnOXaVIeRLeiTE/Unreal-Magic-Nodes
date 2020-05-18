//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///			Copyright 2020 (C) Bruno Xavier B. Leite
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_NodeStyle.h"
#include "MagicNodeKismet_Shared.h"

#include "Fonts/CompositeFont.h"

#include "Styling/CoreStyle.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateTypes.h"
#include "Styling/SlateStyleRegistry.h"

#include "Interfaces/IPluginManager.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MTF_FONT(Name,RelativePath,...) FCompositeFont(Name,FKMGC_NodeStyle::InContent(RelativePath,TEXT(".ttf")),EFontHinting::Default,EFontLoadingPolicy::LazyLoad);
#define IMAGE_BRUSH(RelativePath,...) FSlateImageBrush(FKMGC_NodeStyle::InContent(RelativePath,TEXT(".png")),__VA_ARGS__)
#define BOX_BRUSH(RelativePath,...) FSlateBoxBrush(FKMGC_NodeStyle::InContent(RelativePath,TEXT(".png") ),__VA_ARGS__)
#define TTF_FONT(RelativePath,...) FSlateFontInfo(FKMGC_NodeStyle::InContent(RelativePath,TEXT(".ttf")),__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedPtr<FSlateStyleSet>FKMGC_NodeStyle::StyleSet = nullptr;
TSharedPtr<ISlateStyle>FKMGC_NodeStyle::Get() {return StyleSet;}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FString FKMGC_NodeStyle::InContent(const FString &RelativePath, const TCHAR* Extension) {
	static FString Content = IPluginManager::Get().FindPlugin(TEXT("MagicNode"))->GetContentDir();
	return (Content/RelativePath)+Extension;
}

FName FKMGC_NodeStyle::GetStyleSetName() {
	static FName StyleName(TEXT("KMGC_NodeStyle"));
	return StyleName;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void FKMGC_NodeStyle::Initialize() {
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
	const FVector2D Icon48x48(48.f,48.f);
	const FVector2D Icon64x64(64.f,64.f);
	const FVector2D Icon128x128(128.f,128.f);
	//
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
	//
	/// Icons:
	{
		StyleSet->Set("KMGC.MagicNodeIcon.Small", new IMAGE_BRUSH(TEXT("Icons/MagicNode_128x"),Icon16x16));
		StyleSet->Set("KMGC.MagicNodeIcon", new IMAGE_BRUSH(TEXT("Icons/MagicNode_128x"),Icon128x128));
		//
		StyleSet->Set("KMGC.Toolbar.H", new IMAGE_BRUSH(TEXT("UI/KMGC_H_22x"),Icon22x22));
		StyleSet->Set("KMGC.Toolbar.T", new IMAGE_BRUSH(TEXT("UI/KMGC_T_22x"),Icon22x22));
		StyleSet->Set("KMGC.Toolbar.CPP", new IMAGE_BRUSH(TEXT("UI/KMGC_CPP_22x"),Icon22x22));
		StyleSet->Set("KMGC.Toolbar.Compile", new IMAGE_BRUSH(TEXT("UI/KMGC_Compile_40x"),Icon40x40));
		StyleSet->Set("KMGC.Toolbar.SaveScript", new IMAGE_BRUSH(TEXT("UI/KMGC_Save_40x"),Icon40x40));
		StyleSet->Set("KMGC.Toolbar.Database", new IMAGE_BRUSH(TEXT("UI/KMGC_Database_40x"),Icon40x40));
		StyleSet->Set("KMGC.Toolbar.MagicNode", new IMAGE_BRUSH(TEXT("Icons/MagicNode_128x"),Icon40x40));
		StyleSet->Set("KMGC.Toolbar.ReloadScript", new IMAGE_BRUSH(TEXT("UI/KMGC_Reload_40x"),Icon40x40));
		//
		StyleSet->Set("KMGC.Focus", new IMAGE_BRUSH("UI/KMGC_LineFocus_40x",Icon40x40));
		StyleSet->Set("KMGC.Lines", new IMAGE_BRUSH("UI/KMGC_TextHighlight",Icon40x40));
	}
	//
	/// Text Editor:
	{
		StyleSet->Set("KMGC.CodeBlockStyle",TBS_CodeStyle);
		//
		StyleSet->Set("KMGC.SyntaxHighlight.Processor",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(125,125,125,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Container",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(125,235,185,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Variable",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,225,155,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Operator",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(95,255,245,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Normal",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,235,195,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Keyword",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(50,125,255,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.String",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,155,135,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Function",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,105,5,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Number",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(55,255,155,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Comment",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(55,125,65,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Class",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(75,185,245,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Macro",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(225,10,155,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Illegal",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(175,5,5,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Event",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(225,25,25,255))));
		StyleSet->Set("KMGC.SyntaxHighlight.Type",FTextBlockStyle(TBS_CodeStyle).SetColorAndOpacity(FLinearColor(FColor(255,175,5,255))));
	}
	//
	//
	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

void FKMGC_NodeStyle::Shutdown() {
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