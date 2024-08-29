// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK_TextWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_TextWidget : public UUserWidget
{
	GENERATED_BODY()
public :
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ChatText;

	UFUNCTION()
	void SetChatText(FText text);
};
