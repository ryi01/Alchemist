// Fill out your copyright notice in the Description page of Project Settings.


#include "../CHJ/CHJGameModeBase.h"

#include "Illustrated_Guide/GuideObject/IG_InventorySubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

void ACHJGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	// 인벤토리 서브 시스템 가져오기
	UIG_InventorySubsystem* Subsystem = Cast<UIG_InventorySubsystem>(USubsystemBlueprintLibrary::GetWorldSubsystem(this, UIG_InventorySubsystem::StaticClass()));

	if(Subsystem)
	{	// 서브시스템에서 
		Subsystem->AddDefaultItems();
	}
}
