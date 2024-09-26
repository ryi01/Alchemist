// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_PlayerController.h"

#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Kismet/GameplayStatics.h"

void ASYH_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	if(IsLocalController())
	{
		CLientRPC_PlayerName();
	}
}

void ASYH_PlayerController::ServerRPC_PlayerName_Implementation(const FString& PlayerName)
{
	ASYH_MultiPlayer* player = Cast<ASYH_MultiPlayer>(GetPawn());
	if(player)
	{
		player->ServerRPC_SetPlayerName(PlayerName);
	}
}

//  클라이언트가 게임인스턴스에 있는 이름을 서버로 전송
void ASYH_PlayerController::CLientRPC_PlayerName_Implementation()
{
	UGuide_GameInstance* gi = Cast<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(gi)
	{
		ServerRPC_PlayerName(gi->PlayerName);
	}
}
