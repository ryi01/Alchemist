// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "KMK_JsonParseLib.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_JsonParseLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	// static FString JsonParse(const FString& json);
	static FString MakeJson(const TMap<FString, FString> source);
};
