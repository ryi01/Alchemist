// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_QuizWidgetResult.h"

#include "SYH/SYH_MultiPlayer.h"
#include "KMK/PlayerInteractionComponent.h"

void USYH_QuizWidgetResult::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	MultiPlayer = Cast<ASYH_MultiPlayer>(GetOwningPlayerPawn());
	if(Win && Lose && Same)
	{
		Win->SetVisibility(ESlateVisibility::Hidden);
		Lose->SetVisibility(ESlateVisibility::Hidden);
		Same->SetVisibility(ESlateVisibility::Hidden);
		Wait->SetVisibility(ESlateVisibility::Hidden);
		Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetWinVisibility(bool bshow)
{
	FTimerHandle TimerHandle;
	if(bshow)
	{
		Win->SetVisibility(ESlateVisibility::Visible);
		Result->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		if(MultiPlayer) // 요청을 받은 사람
		{
			MultiPlayer->InQuiz = false;
			
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this ,&USYH_QuizWidgetResult::DeleteWinUI, 2.0f, false);
	}
	else
	{
		Win->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetLoseVisibility(bool bshow)
{
	FTimerHandle TimerHandle;
	if(bshow)
	{
		Lose->SetVisibility(ESlateVisibility::Visible);
		Result->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		if(MultiPlayer)
		{
			MultiPlayer->InQuiz = false;
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this ,&USYH_QuizWidgetResult::DeleteLoseUI, 2.0f, false);
	}
	else
	{
		Lose->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetSameVisibility(bool bshow)
{
	FTimerHandle TimerHandle;
	if(bshow)
	{
		Same->SetVisibility(ESlateVisibility::Visible);
		Result->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);

		if(MultiPlayer) // 요청을 받은 사람
		{
			MultiPlayer->InQuiz = false;
		}
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this ,&USYH_QuizWidgetResult::DeleteUI, 2.0f, false);
	}
	else
	{
		Same->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetWaitVisibility(bool bshow)
{
	if(bshow)
	{
		Wait->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		Wait->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::DeleteWinUI()
{
	MultiPlayer->IsWin = true;
	MultiPlayer->TargetPlayer->IsLose = true;
	RemoveFromParent();
	MultiPlayer->interactionComp->CreateMainWidget();
}

void USYH_QuizWidgetResult::DeleteLoseUI()
{
	MultiPlayer->IsLose = true;
	MultiPlayer->TargetPlayer->IsWin = true;
	RemoveFromParent();
	MultiPlayer->interactionComp->CreateMainWidget();
}
void USYH_QuizWidgetResult::DeleteUI()
{
	RemoveFromParent();
	MultiPlayer->interactionComp->CreateMainWidget();
}