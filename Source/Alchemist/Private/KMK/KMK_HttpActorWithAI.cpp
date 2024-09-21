// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_HttpActorWithAI.h"
#include "HttpModule.h"
#include "KMK/KMK_JsonParseLib.h"
#include "KMK/KMK_ChatBotWidget.h"
#include "Components/EditableText.h"
#include "../CHJ/Guide_GameInstance.h"
#include "KMK/KMK_GrabActorComp.h"
#include "Components/WidgetComponent.h"
#include "KMK/KMK_RecommandWidget.h"
#include "KMK/KMK_MakeEleWidget.h"
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

	ReqInitInfo();
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
		ParsecNewInfo(respon, false);
	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}
}

void AKMK_HttpActorWithAI::ReqElement(FString data, UKMK_GrabActorComp* comp)
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	PotComp = comp;
	TMap<FString,FString> maps;
	maps.Add(TEXT("key"),data);

	req->SetURL("https://absolute-logically-hagfish.ngrok-free.app/basic");
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(UKMK_JsonParseLib::MakeJson(maps));
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this,&AKMK_HttpActorWithAI::OnResElement);
	// 서버에 요청

	req->ProcessRequest();
}

void AKMK_HttpActorWithAI::OnResElement(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 성공
		FString respon = Response->GetContentAsString();
		TMap<FString, FString> result = UKMK_JsonParseLib::ResultAlchemistParsec(respon);
		FString s;
		if ( !result.IsEmpty() )
		{
			s += TEXT("결과원소 : ") + result[ TEXT("Result") ] + TEXT("\n") + TEXT("이름 : ") + result[ TEXT("Name") ] + TEXT("\n") + TEXT("사용하는 곳 : ") + TEXT("\n") + result[ TEXT("Using") ] + TEXT("\n");
			PotComp->CreateElementSucced(result[ TEXT("Result") ],s);
			
		}
		else
		{
			PotComp->CreateElementFailed();
		}

	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}
}

void AKMK_HttpActorWithAI::ReqInitInfo()
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	req->SetURL("https://absolute-logically-hagfish.ngrok-free.app/base_info");
	req->SetVerb(TEXT("GET"));
	req->SetHeader(TEXT("content-type"),TEXT("application/json"));
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this,&AKMK_HttpActorWithAI::OnResInitInfo);
	// 서버에 요청

	req->ProcessRequest();
}

void AKMK_HttpActorWithAI::OnResInitInfo(FHttpRequestPtr Request,FHttpResponsePtr Response,bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 성공
		FString respon = Response->GetContentAsString();

		ParsecNewInfo(respon,true);

	}
	else {
		// 실패
		UE_LOG(LogTemp, Warning, TEXT("OnResPostTest Failed..."));
	}

}

void AKMK_HttpActorWithAI::ReqRecommandEle(FString data)
{
	// HTTP 모듈 생성
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	TMap<FString,FString> maps;
	maps.Add(TEXT("key"),data);

	req->SetURL("https://absolute-logically-hagfish.ngrok-free.app/similar_elements");
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"),TEXT("application/json"));
	req->SetContentAsString(UKMK_JsonParseLib::MakeJson(maps));
	// 응답받을 함수를 연결
	req->OnProcessRequestComplete().BindUObject(this,&AKMK_HttpActorWithAI::OnResRecommandEle);
	// 서버에 요청

	req->ProcessRequest();
}

void AKMK_HttpActorWithAI::OnResRecommandEle(FHttpRequestPtr Request,FHttpResponsePtr Response,bool bConnectedSuccessfully)
{
	if ( bConnectedSuccessfully )
	{
		// 성공
		FString respon = Response->GetContentAsString();

		TMap<FString,TMap<FString,FString>> result = UKMK_JsonParseLib::RecommandEleParsec(respon);

		TArray<FString> name;

		if ( !result.IsEmpty() )
		{
			result.GetKeys(name);
			for ( int i = 1; i <= result.Num(); i++ )
			{
				auto* widComp = PotComp->GetOwner()->FindComponentByClass<UWidgetComponent>();
				if ( widComp )
				{
					auto* mainPotWid = Cast<UKMK_MakeEleWidget>(widComp->GetWidget());
					if ( mainPotWid )
					{
						mainPotWid->SetVisibility(ESlateVisibility::Visible);
						auto* wid = Cast<UKMK_RecommandWidget>(CreateWidget(GetWorld(),DetailUIFactory));
						if ( wid )
						{
							FString s = result[ name[ i - 1 ] ][ TEXT("a)") ] + TEXT("\n\n") + TEXT("b)") + result[ name[ i - 1 ] ][ TEXT("b)") ];

							FString english = name[i - 1 ].TrimStartAndEnd(); // 각 섹션의 앞뒤 공백 제거

							// 각 섹션의 제목을 추출 (예: "1. 리튬 (Lithium, Li)")
							FString chemical;
							int32 nameEnd = english.Find(TEXT("("));
							if ( nameEnd != INDEX_NONE )
							{
								name[i-1] = english.Left(nameEnd).TrimStartAndEnd();
								// 섹션 제목 다음 부분만 남김
								chemical = english.Mid(nameEnd).TrimStartAndEnd();
							}
							wid->SetNameAndText(name[ i - 1 ],chemical,s);
							mainPotWid->SetSlotChild(wid, this);
							
							isWidgetOn = true;
						}
					}
				}
			}
		}
	}
	else {
		// 실패
		UE_LOG(LogTemp,Warning,TEXT("Recommand Failed..."));
	}
}

void AKMK_HttpActorWithAI::ParsecNewInfo(FString& respon, bool isInit)
{
	TArray<FString> SectionName = { TEXT("1. 기본 정보"), TEXT("2. 특성"), TEXT("3. 용도"), TEXT("4. 흥미로운 사실") };

	if(!isInit )
	{
		TMap<FString,TMap<FString,FString>> result = UKMK_JsonParseLib::ChatBotParsec(respon,SectionName);
		if ( result.Contains(TEXT("Fail")) )
		{
			HttpUI->MakeChatText(result[ TEXT("Fail") ][ TEXT("Fail") ],1);
			return;
		}
		for ( int i = 0; i < SectionName.Num(); i++ )
		{
			FString arr;
			int count = 0;
			const TMap<FString,FString>& SupMap = result[ SectionName[ i ] ];
			for ( const TPair<FString,FString>& Pair : SupMap )
			{
				FString t = Pair.Key + TEXT("\n") + Pair.Value + TEXT("\n\n");
				if ( i == 0 && count == 0 && result.Contains(TEXT("Header")) )
				{
					arr += result[ TEXT("Header") ][ TEXT("Content") ] + TEXT("\n\n") + t;
					count++;
				}
				else arr += t;
			}
			if ( result.Contains(TEXT("Footer")) )
			{
				arr += TEXT("\n\n") + result[ TEXT("Footer") ][ TEXT("Content") ];
			}
			HttpUI->MakeChatText(arr,1);

		}

		HttpUI->PlayerChat->SetIsEnabled(true);
	}
	else
	{
		TMap<FString,TMap<FString,FString>> result = UKMK_JsonParseLib::InitInfoParsec(respon,SectionName);

		auto* gm = Cast<UGuide_GameInstance>(GetWorld()->GetGameInstance());
		if ( gm )
		{
			gm->SetInitInfo(result, SectionName);
		}
	}
}

