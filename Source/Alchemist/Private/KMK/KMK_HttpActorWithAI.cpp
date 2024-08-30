// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_HttpActorWithAI.h"
#include "HttpModule.h"
#include "KMK/KMK_JsonParseLib.h"
#include "KMK/KMK_ChatBotWidget.h"
#include "Components/EditableText.h"
// Sets default values
AKMK_HttpActorWithAI::AKMK_HttpActorWithAI()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AKMK_HttpActorWithAI::BeginPlay()
{
	Super::BeginPlay();
	////// UI를 생성해서 기억하고싶다.
	//HttpUI = Cast<UKMK_ChatBotWidget>(CreateWidget(GetWorld(), HttpUIFactory));
	//if ( HttpUI )
	//{
	//	HttpUI->AddToViewport();
	//	HttpUI->SetHttpActor(this);
	//}
}

// Called every frame
void AKMK_HttpActorWithAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKMK_HttpActorWithAI::ReqChatBot(FString json)
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	// 요청할 정보를 설정
	TMap<FString, FString> data;
	MyData = json;
	data.Add(TEXT("key"), json);

	req->SetURL("https://absolute-logically-hagfish.ngrok-free.app/posttest");
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKMK_JsonParseLib::MakeJson(data));
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AKMK_HttpActorWithAI::OnResChatBot);
	// 서버에 요청

	req->ProcessRequest();
}


void AKMK_HttpActorWithAI::OnResChatBot(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 성공
		FString respon = Response->GetContentAsString();
		TMap<FString, FString> result = UKMK_JsonParseLib::ChatBotParsec(respon, MyData);
		if ( HttpUI && !result.IsEmpty())
		{
			HttpUI->MakeChatText(FText::FromString(result[ TEXT("산소") ]));
			HttpUI->PlayerChat->SetIsEnabled(true);
		}
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}
}

void AKMK_HttpActorWithAI::ReqElement(TMap<FString, FString> data)
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();

	req->SetURL("https://absolute-logically-hagfish.ngrok-free.app/posttest");
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKMK_JsonParseLib::MakeJson(data));
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this, &AKMK_HttpActorWithAI::OnResChatBot);
	// 서버에 요청

	req->ProcessRequest();
}

void AKMK_HttpActorWithAI::OnResElement(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
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

