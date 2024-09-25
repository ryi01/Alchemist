// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_QuizWidgetResult.h"

#include "Components/Image.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SYH/SYH_MultiPlayer.h"
#include "KMK/PlayerInteractionComponent.h"

void USYH_QuizWidgetResult::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	MultiPlayer = Cast<ASYH_MultiPlayer>(GetOwningPlayerPawn());
	if(WinImage && LoseImage && SameImage)
	{
		WinImage->SetVisibility(ESlateVisibility::Hidden);
		LoseImage->SetVisibility(ESlateVisibility::Hidden);
		SameImage->SetVisibility(ESlateVisibility::Hidden);
		Wait->SetVisibility(ESlateVisibility::Hidden);
		Result->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetWinVisibility(bool bshow)
{
	FTimerHandle TimerHandle;
	if(bshow)
	{
		WinImage->SetVisibility(ESlateVisibility::Visible);
		Result->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(false);
		if(MultiPlayer) // 요청을 받은 사람
		{
			MultiPlayer->InQuiz = false;
			MultiPlayer->TargetPlayer->InQuiz = false;
		}
		UGameplayStatics::PlaySound2D(GetWorld(),WinResult);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this ,&USYH_QuizWidgetResult::DeleteWinUI, 2.0f, false);
	}
	else
	{
		WinImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetLoseVisibility(bool bshow)
{
	FTimerHandle TimerHandle;
	if(bshow)
	{
		LoseImage->SetVisibility(ESlateVisibility::Visible);
		Result->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(false);
		if(MultiPlayer)
		{
			MultiPlayer->InQuiz = false;
			MultiPlayer->TargetPlayer->InQuiz = false;
		}
		UGameplayStatics::PlaySound2D(GetWorld(),LoseResult);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this ,&USYH_QuizWidgetResult::DeleteLoseUI, 2.0f, false);
	}
	else
	{
		LoseImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void USYH_QuizWidgetResult::SetSameVisibility(bool bshow)
{
	FTimerHandle TimerHandle;
	if(bshow)
	{
		SameImage->SetVisibility(ESlateVisibility::Visible);
		Result->SetVisibility(ESlateVisibility::Visible);
		PlayerController->SetShowMouseCursor(false);

		if(MultiPlayer) // 요청을 받은 사람
		{
			MultiPlayer->InQuiz = false;
			MultiPlayer->TargetPlayer->InQuiz = false;
		}

		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this ,&USYH_QuizWidgetResult::DeleteUI, 2.0f, false);
	}
	else
	{
		SameImage->SetVisibility(ESlateVisibility::Hidden);
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
	PlayerController->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::PlaySound2D(GetWorld(),WinSound);
	RemoveFromParent();
	MultiPlayer->interactionComp->CreateMainWidget();
}

void USYH_QuizWidgetResult::DeleteLoseUI()
{
	MultiPlayer->IsLose = true;
	MultiPlayer->TargetPlayer->IsWin = true;
	PlayerController->SetInputMode(FInputModeGameOnly());
	UGameplayStatics::PlaySound2D(GetWorld(),LoseSound);
	RemoveFromParent();
	MultiPlayer->interactionComp->CreateMainWidget();
}
void USYH_QuizWidgetResult::DeleteUI()
{
	MultiPlayer->IsSame = true;
	MultiPlayer->TargetPlayer->IsSame = true;
	PlayerController->SetInputMode(FInputModeGameOnly());
	RemoveFromParent();
	MultiPlayer->interactionComp->CreateMainWidget();
}