// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_SlotsWidget.h"
#include "Guide_IconWidget.h"
#include "Components/UniformGridPanel.h"

UGuide_SlotsWidget::UGuide_SlotsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UGuide_IconWidget> FindSlotWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/CHJ/Illerstrated_Guide/Guide_Widget/WBP_Guide_IconWidget.WBP_Guide_IconWidget_C'"));
	if (FindSlotWidgetClass.Succeeded())
	{
		IconWidgetClass = FindSlotWidgetClass.Class;
	}
}

void UGuide_SlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const int X_COUNT = 4;
	const int Y_COUNT = 2;

	IconWidgets.SetNum(X_COUNT * Y_COUNT); // 10 X 5 슬롯 위젯 

	for(int32 y=0; y < Y_COUNT; y++)
	{
		for(int32 x = 0; x < X_COUNT; x++)
		{
			int32 index = y * X_COUNT + x; 

			UGuide_IconWidget* iconWidget = CreateWidget<UGuide_IconWidget>(GetOwningPlayer(), IconWidgetClass);
			IconWidgets[index] = iconWidget;
			GridPanel_Slots->AddChildToUniformGrid(iconWidget, y, x);
		}
	}
	
}
