// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SYH_QuizWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_QuizWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Question;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_1;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_2;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_3;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Answer_1;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Answer_2;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Answer_3;
};
