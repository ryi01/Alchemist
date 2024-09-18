// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_MenuWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void USYH_MenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	if(PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
	if(BackButt)
	{
		BackButt->OnClicked.RemoveDynamic(this, &USYH_MenuWidget::OnClickedBackButt); 
		BackButt->OnClicked.AddDynamic(this,&USYH_MenuWidget::OnClickedBackButt);
	}
	if(ExitButt)
	{
		ExitButt->OnClicked.RemoveDynamic(this, &USYH_MenuWidget::OnClickedExitButt); 
		ExitButt->OnClicked.AddDynamic(this,&USYH_MenuWidget::OnClickedExitButt);
	}
}

void USYH_MenuWidget::OnClickedBackButt()
{
	RemoveFromParent();
	PlayerController->SetShowMouseCursor(false);
	PlayerController->SetInputMode(FInputModeGameOnly());
}

void USYH_MenuWidget::OnClickedExitButt()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),PlayerController,EQuitPreference::Quit,false);
}
