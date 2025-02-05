﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_ElementGameActor.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UKMK_ElementGameActor::UKMK_ElementGameActor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_ElementGameActor::BeginPlay()
{
	Super::BeginPlay();
	textWidget = Cast<UKMK_TextWidget>(GetOwner()->GetComponentByClass<UWidgetComponent>()->GetWidget());
	if(textWidget)textWidget->SetVisibility(ESlateVisibility::Hidden);
}


// Called every frame
void UKMK_ElementGameActor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UKMK_ElementGameActor::ChangeMyPos()
{
	isUp = true;
}

