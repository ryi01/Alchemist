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


void AKMK_HttpActorWithAI::ReqChatBot1(FString json)
{
	FString json1 = TEXT("1. 기본 정보\n- 원소 이름: 산소 (Oxid, Oxygen)\n- 원소 기호 또는 화학식: O\n- 원자 번호: 8 (원소의 경우)\n\n2. 특성\n- 물리적 특성: 상온에서 가스 형태로 존재한다.\n- 화학적 특성: 매우 활발한 화학 성질을 가지고 있다. 특히, 다른 대부분의 화합물에 비해 화학 반응성이 뛰어나다.\n\n3. 용도\n- 주요 사용 분야: 호흡과 에너지 생성. 산소는 인간의 호흡에 필수적인 기체이며, 산업적으로는 연료로 사용되어 가스화되거나 전기와 열을 생산하기 위해 연소된다.\n- 일상생활에서의 응용: 산소는 마스크, 풍선, 연기 등 다양한 제품에서 발견된다. 또한, 산소를 함유한 물질들은 약재, 화약 및 연료로 사용된다.\n\n4. 흥미로운 사실\n- 역사적 배경: 산소의 존재를 처음으로 증명한 과학자는 1772년 영국의 과학자 조셉 프라리였다. 하지만, 1776년 덴마크 과학자 얀 로렌트 라이거드가 상당량의 산소를 별도로 분리했다.\n- 특이한 성질: 산소는 생명체의 호흡과 필연적으로 연관된 기체로, 거의 모든 생물이 산소 공급에 의존한다. 산소의 화학적 활성은 매우 높고, 이는 많은 화합물을 구성하는 데 사용된다.");
	TMap<FString,FString> result = UKMK_JsonParseLib::ChatBotParsec(json1, MyData);
	HttpUI->MakeChatText(result[TEXT("1. 기본 정보")],1);
	HttpUI->MakeChatText(result[TEXT("2. 특성")],1);
	HttpUI->MakeChatText(result[TEXT("3. 용도")],1);
	HttpUI->MakeChatText(result[TEXT("4. 흥미로운 사실")],1);
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
			HttpUI->MakeChatText(result[ TEXT("explanation") ],1);
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

