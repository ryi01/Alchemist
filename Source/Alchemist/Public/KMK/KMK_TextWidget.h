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
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* BackTextImage;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* TextVerticalBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* ChatBotTextureBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* ChatBotScrollBox;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UMultiLineEditableTextBox* ResChatText;
	UFUNCTION()
	void SetChatText(FString text, int num = 0);

	UFUNCTION()
	void SetVisibleText(class UVerticalBoxSlot* childSlot, EHorizontalAlignment align, ESlateVisibility visible);
	int count = -1;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UTexture2D*> mats;
};
