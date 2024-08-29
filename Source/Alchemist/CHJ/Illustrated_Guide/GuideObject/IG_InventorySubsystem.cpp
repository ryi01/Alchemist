// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/IG_InventorySubsystem.h"

#include "Guide_ObjectInstance.h"

void UIG_InventorySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
		
}

void UIG_InventorySubsystem::Deinitialize()
{
	Super::Deinitialize();
	
}

void UIG_InventorySubsystem::AddDefaultItems()
{
	TObjectPtr<UGuide_ObjectInstance> Item = NewObject<UGuide_ObjectInstance>();
	Item->Init(100); // 아이템 ID 초기화
	Items.Add(Item); // 아이템 추가 
	
}
