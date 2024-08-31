// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_SlotsWidget.h"

#include "Guide_EntryWidget.h"
#include "Guide_IconWidget.h"
#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Kismet/GameplayStatics.h"

UGuide_SlotsWidget::UGuide_SlotsWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UGuide_IconWidget> FindSlotWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/CHJ/Illerstrated_Guide/Guide_Widget/WBP_Guide_IconWidget.WBP_Guide_IconWidget_C'"));
	if (FindSlotWidgetClass.Succeeded())
	{
		IconWidgetClass = FindSlotWidgetClass.Class;
	}

	ConstructorHelpers::FClassFinder<UGuide_EntryWidget> FindEntryWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/CHJ/Illerstrated_Guide/Guide_Widget/WBP_Guide_EntryWidget.WBP_Guide_EntryWidget_C'"));
	if (FindEntryWidgetClass.Succeeded())
	{
		EntryWidgetClass = FindEntryWidgetClass.Class;
	}
}


void UGuide_SlotsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CreateEmptySlots();
	
	EntryWidgets.SetNum(X_COUNT * Y_COUNT);
	// 게임인스턴스를 가져옴
	auto* GameInstance = CastChecked<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	// 인스턴스의 아이템 갯수만큼 엔트리 만든다
	for (int i = 0 ; i < GameInstance->PictureItems.Num(); i++)
	{
		CreateEntrySlot(i, GameInstance->PictureItems[i]);
	}
	
}

void UGuide_SlotsWidget::CreateEmptySlots()
{
	IconWidgets.SetNum(X_COUNT * Y_COUNT); // 위젯 배열의 크기를 x * y로 설정한다
	//아이템 빈 슬롯 생성
	for(int32 y=0; y < Y_COUNT; y++) // 2
	{
		for(int32 x = 0; x < X_COUNT; x++) // 4
		{
			int32 index = y * X_COUNT + x; // 0 * 4 + 0 
			
			UGuide_IconWidget* iconWidget = CreateWidget<UGuide_IconWidget>(GetOwningPlayer(), IconWidgetClass);
			IconWidgets[index] = iconWidget;
			GridPanel_Slots->AddChildToUniformGrid(iconWidget, y, x); // 그리드 위젯 생성
		}
	}
}

void UGuide_SlotsWidget::CreateEntrySlot(int32 X, int32 Y)
{
	// int32 SlotIndex = Y * X_COUNT + X; // 1차원으로 변형해서 
	// UGuide_EntryWidget* EntryWidget = CreateWidget<UGuide_EntryWidget>(GetOwningPlayer(), EntryWidgetClass);
	// EntryWidgets[SlotIndex] = EntryWidget;
	// GridPanel_Slots->AddChildToUniformGrid(EntryWidget, X, Y); // 그리드 위젯 생성
}

void UGuide_SlotsWidget::CreateEntrySlot(int32 index, FElementDatas& data)
{
	if(EntryWidgets.Num() <= index) return;
	
	UGuide_EntryWidget* EntryWidget = CreateWidget<UGuide_EntryWidget>(GetOwningPlayer(), EntryWidgetClass);
	EntryWidgets[index] = EntryWidget; // 해당 인덱스에 추가

	EntryWidgets[index]->SetElementData(data); // 해당 엔트리 위젯을 바꾼다
	int32 X = index % X_COUNT;
	int32 Y = index / X_COUNT;
	GridPanel_Slots->AddChildToUniformGrid(EntryWidget, Y, X); // 그리드 위젯에 추가
}


