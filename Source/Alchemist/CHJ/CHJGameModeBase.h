// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CHJGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API ACHJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	// 게임 시작됐을때 실행되는 함수
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
};
