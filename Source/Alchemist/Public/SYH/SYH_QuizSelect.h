// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "SYH_QuizSelect.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_QuizSelect : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget))
	UButton* YesButt;
	UPROPERTY(meta = (BindWidget))
	UButton* PauseButt;
	UFUNCTION()
	void OnClickedYesButt();
	UFUNCTION()
	void OnClickedPausebutt();
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY()
	class ASYH_MultiPlayer* MultiPlayer;

	// 상대방을 넘겨받는 함수가 필요
	
	// class UGuide_GameInstance* GameInstance;
	// bool buttonResult = true;
};
