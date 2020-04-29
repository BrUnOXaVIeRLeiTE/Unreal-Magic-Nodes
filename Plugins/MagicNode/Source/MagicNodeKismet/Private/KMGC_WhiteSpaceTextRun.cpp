//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
///		Copyright 2020 (C) Bruno Xavier B. Leite
///		Based on code sample by Epic Games (C) 1998-2020 Epic Games, Inc.
/////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "KMGC_WhiteSpaceTextRun.h"

#include "Fonts/FontMeasure.h"
#include "Framework/Application/SlateApplication.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TSharedRef<FWhiteSpaceTextRun>FWhiteSpaceTextRun::Create(const FRunInfo &InRunInfo, const TSharedRef<const FString> &InText, const FTextBlockStyle &InStyle, const FTextRange &InRange, int32 InSpacesPerTab ) {
	return MakeShareable( new FWhiteSpaceTextRun(InRunInfo,InText,InStyle,InRange,InSpacesPerTab));
}

FVector2D FWhiteSpaceTextRun::Measure(int32 StartIndex, int32 EndIndex, float Scale, const FRunTextContext &TextContext) const {
	const FVector2D ShadowOffsetToApply((EndIndex == Range.EndIndex) ? FMath::Abs(Style.ShadowOffset.X * Scale) : 0.0f, FMath::Abs(Style.ShadowOffset.Y * Scale));
	if ((EndIndex-StartIndex)==0) {return FVector2D(ShadowOffsetToApply.X * Scale,GetMaxHeight(Scale));}
	//
	int32 TabCount = 0;
	for(int32 Index = StartIndex; Index < EndIndex; Index++) {
		if ((*Text)[Index]==TEXT('\t')) {TabCount++;}
	}///
	//
	const TSharedRef<FSlateFontMeasure> FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	FVector2D Size = FontMeasure->Measure(**Text,StartIndex,EndIndex,Style.Font,true,Scale) + ShadowOffsetToApply;
	//
	Size.X -= TabWidth * (float)TabCount * Scale;
	Size.X += SpaceWidth * (float)(TabCount * SpacesPerTab) * Scale;
	//
	return Size;
}

FWhiteSpaceTextRun::FWhiteSpaceTextRun(const FRunInfo &InRunInfo, const TSharedRef<const FString >&InText, const FTextBlockStyle &InStyle, const FTextRange &InRange, int32 InSpacesPerTab) 
: FSlateTextRun(InRunInfo, InText, InStyle, InRange)
, SpacesPerTab(InSpacesPerTab) {
	const TSharedRef<FSlateFontMeasure>FontMeasure = FSlateApplication::Get().GetRenderer()->GetFontMeasureService();
	TabWidth = FontMeasure->Measure(TEXT("\t"),0,1,Style.Font,true,1.0f).X;
	SpaceWidth = FontMeasure->Measure(TEXT(" "),0,1,Style.Font,true,1.0f).X;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
