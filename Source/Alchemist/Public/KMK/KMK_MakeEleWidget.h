// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK_MakeEleWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_MakeEleWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintReadWrite, Category = Http)
	class AKMK_HttpActorWithAI* http;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UHorizontalBox* DetailHorizontalBox;

	UPROPERTY(meta = (BindWidget))
	class UButton* OutButt;

	UFUNCTION()
	void ClickButt();
	UFUNCTION()
	void SetSlotChild(UWidget* wid, class AKMK_HttpActorWithAI* httpComp);
};
