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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(meta = (BindWidget))
	class USlider* ZoomSlider;
	virtual void NativeConstruct() override;
	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Yes;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* No;
	UPROPERTY(meta = (BindWidget))
	class UImage* CameraImage;

	
	UPROPERTY(editanywhere, BlueprintReadWrite, Category="Actor")
	FName ActorTag;

private:
	bool IsTaggedActorInView();
	void UpdateTextVisibility();

};
