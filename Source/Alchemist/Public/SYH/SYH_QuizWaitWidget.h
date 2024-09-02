// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SYH_QuizWaitWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_QuizWaitWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite);
	class UTextBlock* Wait;
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite);
	class UTextBlock* Request;
	UPROPERTY(EditAnywhere, meta = (BindWidget),BlueprintReadWrite);
	class UTextBlock* Reject;

	UFUNCTION(BlueprintCallable)
	void SetWaitVisibility(bool bshow);
	UFUNCTION(BlueprintCallable)
	void SetRequestVisibility(bool bshow);
	UFUNCTION(BlueprintCallable)
	void SetRejectVisibility(bool bshow);
};
