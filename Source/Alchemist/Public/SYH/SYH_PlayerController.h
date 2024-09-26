// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SYH_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API ASYH_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerRPC_PlayerName(const FString& PlayerName);

	UFUNCTION(Client, Reliable)
	void CLientRPC_PlayerName();
};
