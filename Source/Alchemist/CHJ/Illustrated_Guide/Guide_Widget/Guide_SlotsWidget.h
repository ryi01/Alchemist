// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Guide_SlotsWidget.generated.h"

class UUniformGridPanel;
class UGuide_IconWidget;
/**
 * 
 */
UCLASS()
class ALCHEMIST_API UGuide_SlotsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UR1InventorySlotsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UGuide_SlotsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TSubclassOf<UGuide_IconWidget> IconWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UGuide_IconWidget>> IconWidgets;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel_Slots;
};
