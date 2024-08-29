// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_ParsecAIContent.h"

// Sets default values for this component's properties
UKMK_ParsecAIContent::UKMK_ParsecAIContent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_ParsecAIContent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UKMK_ParsecAIContent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Ãªº¿ÀÇ °ª ÆÄ¼½
TMap<FString, FString> UKMK_ParsecAIContent::ChatBotParsec(const FString& json, FString ingredient)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	TMap<FString, FString> result;

	if (FJsonSerializer::Deserialize(reader, response))
	{
		/*FString menu = response->GetStringField(TEXT("food_name"));
		FString recipe = response->GetStringField(TEXT("recipe"));*/
		GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue,FString::Printf(TEXT("AI Connect")));
	}
	return result;
}

TMap<FString, FString> UKMK_ParsecAIContent::ResultAlchemistParsec(const FString& json, FString ResultAlchemist)
{
	TMap<FString, FString> result;
	return result;
}
