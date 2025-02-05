﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
#include "SYH/SYH_MultiPlayer.h"
#include "Interfaces/OnlineSessionInterface.h"
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
	// 상세정보 넣은 곳
	TMap<FString,TMap<FString,FString>> ElementDataMap;
	TArray<FString> keyValue; // 섹션 이름

	void SetInitInfo(TMap<FString,TMap<FString,FString>> data,TArray<FString> key);

	TMap<FString,FString> SetMyDataText(FString myName);

	UFUNCTION()
	FString ParsecItemName(FString name);
	UPROPERTY()
	TArray<FString> correctionTag;

	// --------- session ------------------
	// 세션 생성, 검색, 접속 함수
	UFUNCTION(BlueprintCallable, Category = "Network")
	void Create();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void Find();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void Join();
	// 스팀으로 한글 이름 방을 만들어서 조회하면 한글이 깨지는 이슈 발생!!
	// Base64 인코딩으로 해결하고자함!
	FString PlayerName = TEXT("Alpha");

	FString StringBase64Encode(const FString& str);
	FString StringBase64Decode(const FString& str);
protected:
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
private:
	// 델리게이트 함수
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	
	
};
