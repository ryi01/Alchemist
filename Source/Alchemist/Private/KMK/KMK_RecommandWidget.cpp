﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_RecommandWidget.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"

void UKMK_RecommandWidget::SetNameAndText(const FString& nameText,const FString& engText, const FString& infoText)
{
	Name1->SetText(FText::FromString(nameText));
	EngName->SetText(FText::FromString(engText));
	Info1->SetText(FText::FromString(infoText));
}
