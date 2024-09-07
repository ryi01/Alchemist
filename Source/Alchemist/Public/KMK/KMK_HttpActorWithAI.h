// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "KMK_HttpActorWithAI.generated.h"

UCLASS()
class ALCHEMIST_API AKMK_HttpActorWithAI : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKMK_HttpActorWithAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FString MyData;
	// SendButt을 눌렀을 때, 요청할 함수
	void ReqChatBot(FString json);
	// 응답받을 함수
	void OnResChatBot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	// SendButt을 눌렀을 때, 요청할 함수
	void ReqElement(FString data, class UKMK_GrabActorComp* comp);
	// 응답받을 함수
	void OnResElement(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

	void ReqInitInfo();
	void OnResInitInfo(FHttpRequestPtr Request,FHttpResponsePtr Response,bool bConnectedSuccessfully);

	// 추천 받는 원소
	void ReqRecommandEle(FString data);
	void OnResRecommandEle(FHttpRequestPtr Request,FHttpResponsePtr Response,bool bConnectedSuccessfully);

	void ParsecNewInfo(FString& respon, bool isInit = false);

	UPROPERTY()
	class UKMK_GrabActorComp* PotComp;
	UPROPERTY()
	class UKMK_ChatBotWidget* HttpUI;

	UPROPERTY()
	class UKMK_RecommandWidget* RecUI;

	// 인터페이스를 위한 UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> HttpUIFactory;

};
