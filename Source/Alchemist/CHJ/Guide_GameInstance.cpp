// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Guide_GameInstance.h"

UGuide_GameInstance::UGuide_GameInstance()
{
	//데이터 테이블 붙이기
	 ConstructorHelpers::FObjectFinder<UDataTable> tempDT(TEXT("/Script/Engine.DataTable'/Game/Project/CHJ/Illerstrated_Guide/Data/DT_ElementData.DT_ElementData'"));
	 if (tempDT.Succeeded())
	 {	
	 	UDataTable* dataTable = tempDT.Object;
	
	 	// GetRowNames는 전체 열을 가져옴
	 	TArray<FName> rowNames = dataTable->GetRowNames();
	 	// 열을 순차적으로 추가한다
	 	for (int i = 0; i < rowNames.Num(); i++)
	 	{
	 		// 열을 찾겠다 -> rownames의 i번째 // ContextString : 뭘 포함하고 있는지
	 		FElementDatas* itemData = dataTable->FindRow<FElementDatas>(rowNames[i], TEXT(""));
	 		// 찾은거 defineItem에 넣겠다 (defineItem은 포인터로 선언하지 않아 itemData 주소값으로 값을 가져와야함)
	 		DefineItem.Add(*itemData);
	 	}
	 }
}



void UGuide_GameInstance::Init()
{
	Super::Init();
	//flag를 PictureItems 만큼 생성 / 디폴트 값은 false.
	PictureItemFlags.Init(false, SlotsIndex);
}

void UGuide_GameInstance::TakeItemData(int itemIdx)
{	// DefineItem의 데이터 추가
	PictureItems.Add(DefineItem[itemIdx]);
	// 중복 체크 필요
	 if(PictureItems.IsValidIndex(itemIdx)) // 아이템 인덱스가 유효하다면
	 {
		if(PictureItemFlags[itemIdx] == false) // 플래그 인덱스가 false 라면 
		{
			PictureItemFlags[itemIdx] = true; //true로 만들고 
			PictureItemQueue.Enqueue(itemIdx); // 큐에 해당 인덱스 저장한다.
		}
	 }
}

void UGuide_GameInstance::HandleQuizRequest(ASYH_MultiPlayer* RequestingPlayer, AActor* TargetActor)
{
	UE_LOG(LogTemp, Warning, TEXT("HandleQuizRequest 함수가 호출됨"));
	// target actor 가 valid하면 타겟 클라이언트에게 퀴즈 요청을 전송
	if (IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetActor 이름: %s"), *TargetActor->GetName());
		ClientShowQuiz(Cast<ASYH_MultiPlayer>(TargetActor));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("TargetActor가 유효하지 않음"));
	}
}

void UGuide_GameInstance::ClientShowQuiz_Implementation(ASYH_MultiPlayer* TargetPlayer)
{
	if (TargetPlayer)
	{
		// 클라이언트에서 퀴즈 요청 UI 표시
		TargetPlayer->ShowQuizRequestUI();
	}

}
