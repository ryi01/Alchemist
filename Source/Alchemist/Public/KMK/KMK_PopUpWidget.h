// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK_PopUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_PopUpWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PopUpText;
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite)
	class UButton* YesButt;
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite)
	class UButton* NoButt;
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite)
	class UButton* PauseButt;
	UFUNCTION()
	void SetPopUpText(FText text);
	UFUNCTION()
	void OnClickedYesButt();
	UFUNCTION()
	void OnClickedNobutt();
	UFUNCTION()
	void OnClickedPausebutt();
	void VisibleYesButt();

	UPROPERTY()
	class APlayerController* PlayerController;
};
