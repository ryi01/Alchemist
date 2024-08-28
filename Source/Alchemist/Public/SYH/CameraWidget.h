// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CameraWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class ALCHEMIST_API UCameraWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class USlider* ZoomSlider;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnSliderValueChanged(float Value);
	

};
