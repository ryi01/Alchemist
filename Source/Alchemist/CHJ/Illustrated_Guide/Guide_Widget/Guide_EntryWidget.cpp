// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_EntryWidget.h"

#include "Guide_SlotsWidget.h"
#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Guide_ObjectInstance.h"
#include "Guide_DetailWidget.h"
#include "Components/TextBlock.h"
#include "components/Image.h"
#include "Components/SizeBox.h"

UGuide_EntryWidget::UGuide_EntryWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	 ConstructorHelpers::FClassFinder<UGuide_DetailWidget> FindDetailWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/CHJ/Illerstrated_Guide/Guide_Widget/WBP_Guide_DetailWidget.WBP_Guide_DetailWidget_C'"));
	 if (FindDetailWidgetClass.Succeeded())
	 {
	 	DetailWidgetClass = FindDetailWidgetClass.Class;
	 }
}

void UGuide_EntryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//SizeBox_Transform.Scale = FMath::Lerp(SizeBox_Transform.Scale, SizeBoxScale, InDeltaTime * 5);
	SizeBox_Transform.Scale = FMath::Lerp(StartScale, TargetScale, InDeltaTime);
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

	Text_Count->SetText(FText::GetEmpty());
	Image_Hover->SetRenderOpacity(0.f);

	//DetailWidget = CreateWidget<UGuide_DetailWidget>(GetOwningPlayer(), DetailWidgetClass);

}

void UGuide_EntryWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent); // 마우스 댔을 때
	
	//Image_Hover->SetRenderOpacity(0.1f);
	SetSizeBoxScale(1.2f, 1.2f);
	//SizeBox_Root->SetRenderTransform(SizeBox_Transform);
	//GetWorld()->GetTimerManager().SetTimer(SizeBox_ScaleTimerHandle, this, &UGuide_EntryWidget::UpdateSizeBoxScale, 0.01f, true);
}

void UGuide_EntryWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent); // 마우스 뗐을 때
	
	//Image_Hover->SetRenderOpacity(0.f);
	SetSizeBoxScale(1.0f, 1.0f);
	//SizeBox_Root->SetRenderTransform(SizeBox_Transform);
}

FReply UGuide_EntryWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// if(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	// {
	// 	Reply.DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	// }
	UE_LOG(LogTemp, Warning, TEXT("click"));
	UGuide_DetailWidget* DetailWidget = CreateWidget<UGuide_DetailWidget>(GetOwningPlayer(), DetailWidgetClass);
	DetailWidget->AddToViewport();
	return Reply;
}
// 엘레먼트 데이터 받아서 텍스트, 이미지 바꾼다 
void UGuide_EntryWidget::SetElementData(FElementDatas& Data)
{
	//Text_Count->SetText(FText::FromString( Data.Element_name));
	Image_Icon->SetBrushFromTexture(Data.Element_thumbnail);
}

void UGuide_EntryWidget::SetSizeBoxScale(float X, float Y)
{
	FVector2d NewVec = FVector2d(X, Y);
	SizeBox_Transform.Scale = NewVec;
	SizeBox_Root->SetRenderTransform(SizeBox_Transform);
}

void UGuide_EntryWidget::UpdateSizeBoxScale()
{
	FVector2D CurrentScale = FMath::Lerp(StartScale, TargetScale, LerpAlpha);


	// 현재 스케일 값을 적용
	// FWidgetTransform Transform = SizeBox->RenderTransform;
	// Transform.Scale = CurrentScale;
	// SizeBox->SetRenderTransform(Transform);
	SizeBox_Transform.Scale = CurrentScale;
	SizeBox_Root->SetRenderTransform(SizeBox_Transform);
	// LerpAlpha를 증가시켜 보간을 진행
	LerpAlpha += 0.01f; // 이 값을 조정하여 스케일 변경 속도를 제어

	// 목표 스케일에 도달했을 때 타이머 종료
	if (LerpAlpha >= 1.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(SizeBox_ScaleTimerHandle);
	}
}
