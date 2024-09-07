// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_RecommandWidget.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"

void UKMK_RecommandWidget::SetNameAndText(int num, const FString& nameText,const FString& infoText)
{
	switch ( num )
	{
		case 1:
			Name1->SetText(FText::FromString(nameText));
			Info1->SetText(FText::FromString(infoText));
			break;
		case 2:
			Name2->SetText(FText::FromString(nameText));
			Info2->SetText(FText::FromString(infoText));
			break;
		case 3:
			Name3->SetText(FText::FromString(nameText));
			Info3->SetText(FText::FromString(infoText));
			break;
	}
    
}
