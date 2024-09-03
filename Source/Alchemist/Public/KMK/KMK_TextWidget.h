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
	class UVerticalBox* TextVerticalBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UMultiLineEditableTextBox* ResChatText;
	UFUNCTION()
    void SetChatText(FString text);
	int count = -1;
};
