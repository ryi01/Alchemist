// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYH_StartWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_StartWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY(EditAnywhere,meta = (BindWidget))
	class UButton* CreateButt;
	
	UPROPERTY(EditAnywhere,meta = (BindWidget))
	class UButton* JoinButt;

	UPROPERTY(EditAnywhere,meta = (BindWidget))
	class UButton* PauseButt;
	
	UFUNCTION()
	void OnClickedCreateButt();
	UFUNCTION()
	void OnClickedJoinButt();
	UFUNCTION()
	void OnClickedPauseButt();
};
