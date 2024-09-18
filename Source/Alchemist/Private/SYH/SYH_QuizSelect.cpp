
// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_QuizSelect.h"

#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "SYH/SYH_MultiPlayer.h"

void USYH_QuizSelect::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->SetShowMouseCursor(true);
	MultiPlayer = Cast<ASYH_MultiPlayer>(GetOwningPlayerPawn()); // 현재 요청을 받은 사람
	// GameInstance = CastChecked<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (YesButt)
	{
		YesButt->OnClicked.AddDynamic(this, &USYH_QuizSelect::OnClickedYesButt);
	}
	if (PauseButt)
	{
		PauseButt->OnClicked.AddDynamic(this, &USYH_QuizSelect::OnClickedPausebutt);
	}
}
//
void USYH_QuizSelect::OnClickedYesButt()
{
	// yes button을 누르면 퀴즈화면으로 넘어가고 싶다.
	// GameInstance->SelectResult = true;
	// 요청을 받은 사람(this)의 화면에 퀴즈화면을 띄우고 싶다.
	RemoveFromParent();
	PlayerController->SetInputMode(FInputModeGameOnly());
	if(MultiPlayer) // 요청을 받은 사람
	{
		MultiPlayer->InQuiz = true;
		MultiPlayer->ServerRPC_AcceptQuiz();
	}
	
}

void USYH_QuizSelect::OnClickedPausebutt()
{
	RemoveFromParent();
	PlayerController->SetInputMode(FInputModeGameOnly());
	if(MultiPlayer) // 요청을 받은 사람
	{
		MultiPlayer->InQuiz = false;
		MultiPlayer->ServerRPC_RejectQuiz();
	}
		// 요청 받은 사람이 no button을 누르면 UI가 꺼지고 요청을 보낸 사람에게 거절하였다는 UI를 띄우고 싶다.
	// if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer)
	// {
	// 	// 서버 코드
	// 	RemoveFromParent();
	// 	PlayerController->SetInputMode(FInputModeGameOnly());
	// 	if(MultiPlayer) // 요청을 받은 사람
	// 	{
	// 		MultiPlayer->InQuiz = false;
	// 		MultiPlayer->ServerRPC_RejectQuiz();
	// 	}
	// 	UE_LOG(LogTemp,Error,TEXT("server"));
	// }
	// else
	// {
	// 	// 클라이언트 코드
	// 	RemoveFromParent();
	// 	PlayerController->SetInputMode(FInputModeGameOnly());
	// 	if(MultiPlayer)
	// 	{
	// 		MultiPlayer->InQuiz = false;
	// 		MultiPlayer->TargetPlayer->ClientRPC_ShowQuizReject();
	// 	}
	// 	UE_LOG(LogTemp,Error,TEXT("client"));
	// }
}
