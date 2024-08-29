// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Guide_SlotsWidget.generated.h"

class UCanvasPanel;
class UGuide_ObjectInstance;
class UGuide_EntryWidget;
class UUniformGridPanel;
class UGuide_IconWidget;
/**
 * 슬롯츠 위젯은 그리드 판넬이 있는 슬롯들의 모음 위젯
 */
UCLASS()
class ALCHEMIST_API UGuide_SlotsWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UR1InventorySlotsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UGuide_SlotsWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void OnInventoryEntryChanged(const FIntPoint& ItemSlotPos, TObjectPtr<UGuide_ObjectInstance> Item);
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY()
	TSubclassOf<UGuide_IconWidget> IconWidgetClass;

	UPROPERTY()
	TArray<TObjectPtr<UGuide_IconWidget>> IconWidgets;

	UPROPERTY()
	TSubclassOf<UGuide_EntryWidget> EntryWidgetClass;
	UPROPERTY()
	TArray<TObjectPtr<UGuide_EntryWidget>> EntryWidgets;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> GridPanel_Slots;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel_Entries;
	
	const int X_COUNT = 4;
	const int Y_COUNT = 2;
};
