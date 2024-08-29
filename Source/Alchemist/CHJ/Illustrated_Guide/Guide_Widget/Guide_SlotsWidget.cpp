// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_SlotsWidget.h"

#include "Guide_EntryWidget.h"
#include "Guide_IconWidget.h"
#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/IG_InventorySubsystem.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/UniformGridPanel.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

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
	

	IconWidgets.SetNum(X_COUNT * Y_COUNT); // 위젯 배열의 크기를 x * y로 설정한다
	//아이템 빈 슬롯 생성
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

	EntryWidgets.SetNum(X_COUNT * Y_COUNT);
	UIG_InventorySubsystem* Subsystem = Cast<UIG_InventorySubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UIG_InventorySubsystem::StaticClass()));

	const TArray<TObjectPtr<UGuide_ObjectInstance>>& Items = Subsystem->GetItems();

	for(int32 i=0; i<Items.Num(); i++)
	{
		const TObjectPtr<UGuide_ObjectInstance>& Item = Items[i];
		FIntPoint ItemSlotPos = FIntPoint(i % X_COUNT, i / X_COUNT);
		OnInventoryEntryChanged(ItemSlotPos, Item);
	}
}

void UGuide_SlotsWidget::OnInventoryEntryChanged(const FIntPoint& ItemSlotPos, TObjectPtr<UGuide_ObjectInstance> Item)
{
	// [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
	// [ ][1,1][ ][ ][ ][ ][ ][ ][ ][ ] -> [ ][1*10+1][ ][ ][ ][ ][ ][ ][ ]
	// [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
	// 예를 들어 1,1 칸에 넣고싶으면, 1차원으로 변형해서 11번째 칸이라고 바꿔준다
	int32 SlotIndex = ItemSlotPos.Y * X_COUNT + ItemSlotPos.X;

	if(UGuide_EntryWidget* EntryWidget = EntryWidgets[SlotIndex])
	{
		if(Item == nullptr)
		{
			CanvasPanel_Entries->RemoveChild(EntryWidget);
			EntryWidgets[SlotIndex] = nullptr;
		}
	}
	else
	{
		EntryWidget = CreateWidget<UGuide_EntryWidget>(GetOwningPlayer(), EntryWidgetClass);
		EntryWidgets[SlotIndex] = EntryWidget;

		UCanvasPanelSlot* CanvasPanelSlot = CanvasPanel_Entries->AddChildToCanvas(EntryWidget);
		CanvasPanelSlot->SetAutoSize(true);
		CanvasPanelSlot->SetPosition(FVector2D(ItemSlotPos.X * 50, ItemSlotPos.Y * 50));

		EntryWidget->Init(this, Item, 1);

	}
}
