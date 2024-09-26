// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_StartWidget.h"

#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Kismet/GameplayStatics.h"


void USYH_StartWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	if (PlayerController)
	{
		// 마우스 커서 보이기 설정
		PlayerController->SetShowMouseCursor(true);

		// 입력 모드 설정 (UI와 게임 모두 입력받을 수 있도록 설정)
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());  // 현재 위젯에 포커스 설정
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);  // 마우스 잠금 모드 설정
		PlayerController->SetInputMode(InputMode);
	}

	UGuide_GameInstance* GameInstance = Cast<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(CreateButt)
	{
		CreateButt->OnClicked.AddDynamic(this, &USYH_StartWidget::OnClickedCreateButt);
	}
	if(JoinButt)
	{
		JoinButt->OnClicked.AddDynamic(this, &USYH_StartWidget::OnClickedJoinButt);
	}
}

void USYH_StartWidget::OnClickedCreateButt()
{
	if(UGuide_GameInstance* GameInstance = Cast<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		FInputModeGameOnly input;
		PlayerController->SetInputMode(input);
		PlayerController->SetShowMouseCursor(false);
		GameInstance->PlayerName = Edit_PlayerName->GetText().ToString();
		GameInstance->Create();
	}
}

void USYH_StartWidget::OnClickedJoinButt()
{
	if(UGuide_GameInstance* GameInstance = Cast<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		FInputModeGameOnly input;
    	PlayerController->SetInputMode(input);
    	PlayerController->SetShowMouseCursor(false);
		GameInstance->PlayerName = Edit_PlayerName->GetText().ToString();
		GameInstance->Find();
	}
}

void USYH_StartWidget::OnClickedPauseButt()
{
	UKismetSystemLibrary::QuitGame(GetWorld(),PlayerController,EQuitPreference::Quit,false);
}
