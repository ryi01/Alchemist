// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
#include "Guide_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class ALCHEMIST_API UGuide_GameInstance : public UGameInstance
{
	GENERATED_BODY()
 public:
 UGuide_GameInstance();
	
protected:
	virtual void Init() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FElementDatas> DefineItem;
};
