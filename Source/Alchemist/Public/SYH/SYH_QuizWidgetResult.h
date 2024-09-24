// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SYH_QuizWidgetResult.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_QuizWidgetResult : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY()
	class ASYH_MultiPlayer* MultiPlayer;
	UPROPERTY(EditAnywhere,meta=(bindWidget));
	class UImage* WinImage;
	UPROPERTY(EditAnywhere,meta=(bindWidget));
	class UImage* LoseImage;
	UPROPERTY(EditAnywhere,meta=(bindWidget));
	class UImage* SameImage;
	UPROPERTY(EditAnywhere,meta=(bindWidget));
	class UTextBlock* Wait;
	UPROPERTY(EditAnywhere,meta=(bindWidget));
	class UTextBlock* Result;
	UFUNCTION()
	void SetWinVisibility(bool bshow);
	UFUNCTION()
	void SetLoseVisibility(bool bshow);
	UFUNCTION()
	void SetSameVisibility(bool bshow);
	UFUNCTION()
	void SetWaitVisibility(bool bshow);
	UFUNCTION()
	void DeleteWinUI();
	UFUNCTION()
	void DeleteLoseUI();
	UFUNCTION()
	void DeleteUI();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Sound")
	class USoundBase* WinSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Sound")
	class USoundBase* LoseSound;
};
