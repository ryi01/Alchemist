// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UMissionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget ))
	class UTextBlock* SecText;
	UPROPERTY(meta = (BindWidget ))
	class UImage* MissionImage; 
	UPROPERTY(meta = (BindWidget ))
	class UHorizontalBox* TimeHorizonBox;
	UPROPERTY(meta = (BindWidget ))
	class USizeBox* TimeBox;
	UPROPERTY(meta = (BindWidget ))
	class UImage* Click1Image;
	UFUNCTION()
	void SetMissionText(int32 index);

	int32 num = 0;

    UPROPERTY(EditDefaultsOnly)
    TArray<UTexture2D*> missionArray;

	UFUNCTION(BlueprintCallable)
	void SetClickImageVisi(ESlateVisibility visible);

	UFUNCTION()
	void SetTimerEvent(int32 t);

	UFUNCTION()
	void HiddenTime();
};
