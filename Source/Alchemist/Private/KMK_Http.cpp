// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_Http.h"
#include "HttpModule.h"
#include "../Public/KMK_JsonParseLib.h"

// Sets default values
AKMK_Http::AKMK_Http()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKMK_Http::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKMK_Http::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKMK_Http::ReqPostTest(FString json)
{
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	FString ServerURL = "http://192.168.0.53:8080/test";
	// 요청할 정보를 설정
	req->SetURL(ServerURL);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);
	// 요청할 정보를 설정
	TMap<FString, FString> data;
	data.Add(TEXT("key"), json);
	FString j = UKMK_JsonParseLib::MakeJson(data);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *j);
	req->SetContentAsString(j);
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AKMK_Http::OnResPostTest);
	// 서버에 요청

	req->ProcessRequest();
}

void AKMK_Http::OnResPostTest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 성공
		FString result = Response->GetContentAsString();
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}
}

