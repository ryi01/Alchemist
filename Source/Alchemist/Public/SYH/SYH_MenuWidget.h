// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYH_MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_MenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class APlayerController* PlayerController;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButt;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButt;

	UFUNCTION()
	void OnClickedBackButt();
	UFUNCTION()
	void OnClickedExitButt();
	
};
