// Fill out your copyright notice in the Description page of Project Settings.

#include "KMK/KMK_SingleIntaraction.h"
#include "Blueprint/UserWidget.h"



// Sets default values for this component's properties
UKMK_SingleIntaraction::UKMK_SingleIntaraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}
// Called when the game starts
void UKMK_SingleIntaraction::BeginPlay()
{
	Super::BeginPlay();
	// 오너의 widget 들고오기
	auto* owner = GetOwner()->FindComponentByClass< UWidgetComponent>();
	if (owner)
	{
		owner->SetWidgetClass(textWidget->GetClass());
		if (owner->GetWidget() != nullptr)
		{
			textWidget = Cast<UKMK_TextWidget>(owner->GetWidget());
			if (textWidget)
			{
				textWidget->SetChatText(FText::GetEmpty());
			}
		}
	}
}
// Called every frame
void UKMK_SingleIntaraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UKMK_SingleIntaraction::CreatePlayerWidget(bool bMake, int num = 0)
{
	auto* wid = CreateWidget(GetWorld(), widgetFact);
	// 책상 클릭시 카메라 변경
	if ( bMake )
	{
		// 데스크 카메라 활성화
		wid->AddToViewport(num);
	}
	else
	{
		// 플레이어 카메라 활성화
		wid->RemoveFromParent();
	}
}

