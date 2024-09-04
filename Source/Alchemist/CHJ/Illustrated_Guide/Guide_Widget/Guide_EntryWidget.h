// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
#include "Blueprint/UserWidget.h"
#include "Guide_EntryWidget.generated.h"

class UGuide_DetailWidget;
class UGuide_ObjectInstance;
class UImage;
class UTextBlock;
class USizeBox;
class UGuide_SlotsWidget;
/**
 * 아이템 하나를 그려주는 위젯
 */
UCLASS()
class ALCHEMIST_API UGuide_EntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UGuide_EntryWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
public:
	void Init(UGuide_SlotsWidget* InSlotsWidget, UGuide_ObjectInstance* InItemInstance, int32 InItemCount);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	
private: // inpoint -> 인트 두개를 묶어 사용
	FIntPoint CachedFromSlotPos = FIntPoint::ZeroValue;
	FVector2d CachedDeltaWidgetPos = FVector2d::ZeroVector;
	int32 ItemCount = 0;
protected:
	UPROPERTY()
	TObjectPtr<UGuide_SlotsWidget> SlotsWidget;
	
	UPROPERTY()
	TObjectPtr<UGuide_ObjectInstance> ItemInstance;
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Count;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Hover;

protected:
	FWidgetTransform SizeBox_Transform;
	FVector2d SizeBoxScale = FVector2D(1.3f, 1.3f);
	FVector2D StartScale = FVector2D(1.0f, 1.0f); // 시작 스케일
	FVector2D TargetScale = FVector2D(1.3f, 1.3f); // 목표 스케일
	float LerpAlpha = 0.0f;
	FTimerHandle SizeBox_ScaleTimerHandle;
public:
	UPROPERTY(EditAnywhere)
	TArray<FElementDatas> MyElementDatas;

	void SetElementData(FElementDatas& Data);

	void SetSizeBoxScale(float X, float Y);
	void UpdateSizeBoxScale();

protected:
	//UPROPERTY()
	//TObjectPtr<UGuide_DetailWidget> DetailWidget;
	UPROPERTY()
	TSubclassOf<UGuide_DetailWidget> DetailWidgetClass;
};
