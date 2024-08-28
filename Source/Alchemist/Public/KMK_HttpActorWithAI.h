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

	// ai�� url ���� ����
	UPROPERTY(EditAnywhere, Category = "AI_URL")
	FString Aurl;

	// SendButt�� ������ ��, ��û�� �Լ�
	void ReqPostAI(FString json);
	// ������� �Լ�
	void OnResPostAi(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
};
