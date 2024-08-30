// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_EntryWidget.h"

#include "Guide_SlotsWidget.h"
#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Guide_ObjectInstance.h"
#include "Components/TextBlock.h"
#include "components/Image.h"
#include "Components/SizeBox.h"

UGuide_EntryWidget::UGuide_EntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGuide_EntryWidget::Init(UGuide_SlotsWidget* InSlotsWidget, UGuide_ObjectInstance* InItemInstance,
	int32 InItemCount)
{
	SlotsWidget = InSlotsWidget;
	ItemInstance = InItemInstance;
	ItemCount = InItemCount;
}

void UGuide_EntryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//Text_Count->SetText(FText::GetEmpty());
}

void UGuide_EntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent); // 마우스 댔을 때

	Image_Hover->SetRenderOpacity(1.f);
}

void UGuide_EntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent); // 마우스 뗐을 때
	
	Image_Hover->SetRenderOpacity(0.f);
}

FReply UGuide_EntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply;
}
// 엘레먼트 데이터 받아서 텍스트, 이미지 바꾼다 
void UGuide_EntryWidget::SetElementData(FElementDatas& Data)
{
	Text_Count->SetText(FText::FromString( Data.Element_name));
	Image_Icon->SetBrushFromTexture(Data.Element_thumbnail);
}
