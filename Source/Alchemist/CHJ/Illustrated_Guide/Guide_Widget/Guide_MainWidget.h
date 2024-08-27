// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Guide_MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UGuide_MainWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
};
