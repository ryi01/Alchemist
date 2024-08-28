// Fill out your copyright notice in the Description page of Project Settings.

#include "Blueprint/UserWidget.h"
#include "KMK_SingleIntaraction.h"



// Sets default values for this component's properties
UKMK_SingleIntaraction::UKMK_SingleIntaraction()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	if(widgetFact != nullptr) OnCreateMyWidget(bClickActor, widgetFact);
	if (textWidget != nullptr)OnCreateNameWidget(bMouseOnActor);
}


