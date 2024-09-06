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

	static TMap<FString,TMap<FString,FString>>  ChatBotParsec(const FString& json, TArray<FString> ResultChatBot);
	static TMap<FString,TMap<FString,FString>>  InitInfoParsec(const FString& json, TArray<FString> Sections);

	static TMap<FString,FString>  ResultAlchemistParsec(const FString& json);

	static TMap<FString,FString> SplitSectionLight(const FString& json,TArray<FString> Sections);
	static TMap<FString,TMap<FString,FString>> SplitSection(const FString& json,TArray<FString> Sections);
};
