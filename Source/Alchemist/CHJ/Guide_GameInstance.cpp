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
	
}
