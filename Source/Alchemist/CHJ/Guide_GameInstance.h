// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
#include "SYH/SYH_MultiPlayer.h"
#include "Guide_GameInstance.generated.h"

/**
 * 
 */

UCLASS()
class ALCHEMIST_API UGuide_GameInstance : public UGameInstance
{
	GENERATED_BODY()
 public:
 UGuide_GameInstance();
	
protected:
	virtual void Init() override;
		
public:
	// 인스턴스에서 아이템 데이터 가져온다.
	void TakeItemData(int itemIdx);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FElementDatas> DefineItem;

	TArray<FElementDatas> PictureItems;

	TArray<bool> PictureItemFlags;
	TQueue<int32> PictureItemQueue; // 큐에 해당 인덱스 담아서 중복 체크한다.

	int32 SlotsIndex = 8;

public:
	// 서버가 요청을 받았을 때 호출되는 함수
	void HandleQuizRequest(ASYH_MultiPlayer* RequestingPlayer, AActor* TargetActor);

	// 클라이언트에게 퀴즈 UI를 표시하라는 메시지 전송
	UFUNCTION(Client, Reliable)
	void ClientShowQuiz(ASYH_MultiPlayer* TargetPlayer);
};
