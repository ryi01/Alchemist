// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Guide_DetailWidget.generated.h"

class UTextBlock;
class USizeBox;
/**
 * 
 */
UCLASS()
class ALCHEMIST_API UGuide_DetailWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UGuide_DetailWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Detail;
};
