// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK_RecommandWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_RecommandWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name1;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name2;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Name3;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableText* Info1;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableText* Info2;
	UPROPERTY(meta = (BindWidget))
	class UMultiLineEditableText* Info3;

	UFUNCTION()
	void SetNameAndText(int num,const FString& nameText,const FString& infoText);
};
