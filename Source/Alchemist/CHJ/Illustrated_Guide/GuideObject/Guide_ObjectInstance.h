// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Guide_ObjectInstance.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class E_IG_ElementTypes : uint8 // 원소 타입 Enum
{
	One,
	Two,
	Three,
	Four,
	Five,

	Count	UMETA(Hidden)
};

UCLASS(BlueprintType)
class ALCHEMIST_API UGuide_ObjectInstance : public UObject
{
	GENERATED_BODY()

	UGuide_ObjectInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
public:
	void Init(int32 InitObjectID);
public:
	UPROPERTY()
	int32 IG_ObjectID = 0; // 오브젝트 ID

	UPROPERTY()
	E_IG_ElementTypes ElementTypes = E_IG_ElementTypes::One;
};
