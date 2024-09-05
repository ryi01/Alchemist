// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_QuizWaitWidget.h"

#include "Components/TextBlock.h"


void USYH_QuizWaitWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USYH_QuizWaitWidget::SetWaitVisibility(bool bshow)
{
	if (Wait)
	{
		ESlateVisibility NewVisibility = bshow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		Wait->SetVisibility(NewVisibility);

		// UE_LOG(LogTemp, Warning, TEXT("Wait visibility set to: %s"), *UEnum::GetValueAsString(NewVisibility));
	}
}

void USYH_QuizWaitWidget::SetRequestVisibility(bool bshow)
{
	if (Request)
	{
		ESlateVisibility NewVisibility = bshow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		Request->SetVisibility(NewVisibility);

		// UE_LOG(LogTemp, Warning, TEXT("Request visibility set to: %s"), *UEnum::GetValueAsString(NewVisibility));
	}
}

void USYH_QuizWaitWidget::SetRejectVisibility(bool bshow)
{
	if(Reject && bshow)
	{
		Reject->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Reject->SetVisibility(ESlateVisibility::Hidden);
	}
}

