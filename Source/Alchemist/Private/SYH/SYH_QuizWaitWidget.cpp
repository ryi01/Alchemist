// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_QuizWaitWidget.h"

#include "Components/TextBlock.h"
#include "SYH/SYH_MultiPlayer.h"
#include "KMK/PlayerInteractionComponent.h"
#include "KMK/MissionWidget.h"


void USYH_QuizWaitWidget::NativeConstruct()
{
	Super::NativeConstruct();
	MultiPlayer = Cast<ASYH_MultiPlayer>(GetOwningPlayerPawn());
}

void USYH_QuizWaitWidget::SetWaitVisibility(bool bshow)
{
	if (Wait)
	{
		ESlateVisibility NewVisibility = bshow ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		Wait->SetVisibility(NewVisibility);
		if ( !bshow )MultiPlayer->interactionComp->CreateMainWidget();
		else MultiPlayer->interactionComp->DeleteMainWidget();
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
		MultiPlayer->interactionComp->DeleteMainWidget();
	}
	else
	{
		Reject->SetVisibility(ESlateVisibility::Hidden);
		MultiPlayer->interactionComp->CreateMainWidget();
	}
}

