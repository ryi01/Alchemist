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
	if ( !bshow )MultiPlayer->interactionComp->CreateMainWidget();
	else MultiPlayer->interactionComp->DeleteMainWidget();
	if(Wait && bshow)
	{
		Wait->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Wait->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWaitWidget::SetRequestVisibility(bool bshow)
{
	if ( !bshow )MultiPlayer->interactionComp->CreateMainWidget();
	else MultiPlayer->interactionComp->DeleteMainWidget();
	if(Request && bshow)
	{
		Request->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Request->SetVisibility(ESlateVisibility::Hidden);
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
