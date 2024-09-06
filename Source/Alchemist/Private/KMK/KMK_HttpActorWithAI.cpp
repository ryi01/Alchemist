// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_HttpActorWithAI.h"
#include "HttpModule.h"
#include "KMK/KMK_JsonParseLib.h"
#include "KMK/KMK_ChatBotWidget.h"
#include "Components/EditableText.h"
#include "../CHJ/Guide_GameInstance.h"
#include "KMK/KMK_GrabActorComp.h"
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


void AKMK_HttpActorWithAI::ReqChatBot1(FString json)
{
	FString json1 = TEXT("안녕하세요! 산소에 대해 자세히 설명해 드릴게요.\n\n1. 기본 정보:\n   - 원소 이름: 산소 (Oxygen)\n   - 원소 기호: O\n   - 원자 번호: 8\n\n2. 특성:\n   - 물리적 특성: 산소는 상온에서 기체 상태예요. 색깔이 없고, 냄새도 없어요. 그래서 우리가 공기 중에서 느끼지 못하지만, 항상 우리 주위에 있어요.\n   - 화학적 특성: 산소는 매우 활성적인 원소예요. 다른 원소와 쉽게 결합해서 화합물을 만들 수 있어요. 특히, 불이 탈 때 산소가 필요해요.\n\n3. 용도:\n   - 주요 사용 분야: 산소는 생명체가 숨쉬는 데 꼭 필요한 기체예요. 또한, 산업에서도 많이 사용되죠. 예를 들어, 메탈을 녹이거나 합성하는 과정에서 사용돼요.\n   - 일상생활에서의 응용: 우리가 숨을 쉴 때 필요한 공기 속에 산소가 포함되어 있어요. 그리고 의료 분야에서도 환자에게 산소를 공급하는 데 사용해요.\n\n4. 흥미로운 사실:\n   - 역사적 배경이나 발견 과정: 산소는 18세기 중반에 스웨덴의 화학자 카를 빌헬름 셀리우스와 영국의 조지프 프리스틀리가 독립적으로 발견했어요. 산소라는 이름은 라틴어 'oxygène'에서 유래했답니다.\n   - 특이한 성질이나 현상: 산소가 부족하면 생명체는 제대로 활동할 수 없어요. 그래서 고산지대에서는 산소가 적어서 숨쉬기 힘든 경우가 많답니다. 이런 이유로 산소는 우리에게 정말 중요한 원소예요!\n\n이렇게 산소에 대해 알아보았어요. 궁금한 점이 있으면 언제든지 물어봐 주세요!");
	TArray<FString> SectionName = { TEXT("1. 기본 정보"), TEXT("2. 특성"), TEXT("3. 용도"), TEXT("4. 흥미로운 사실") };
	TMap<FString,TMap<FString,FString>> result = UKMK_JsonParseLib::ChatBotParsec(json1,SectionName);
	for ( int i = 0; i < SectionName.Num(); i++ )
	{
		FString arr;
		const TMap<FString, FString>& SupMap = result[SectionName[i]];
		for ( const TPair<FString, FString>& Pair : SupMap )
		{
			FString t = Pair.Key + "\n" + Pair.Value + "\n\n";
			arr += t;
		}
		HttpUI->MakeChatText(arr,1);
	}
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
		if ( result.IsEmpty() )
		{
			return;
		}
		FString s;

		s += TEXT("결과원소 : ") + result[ TEXT("Result") ] + TEXT("\n") + TEXT("이름 : ") + result[ TEXT("Name") ] + TEXT("\n") + TEXT("사용하는 곳 : ") + TEXT("\n") + result[ TEXT("Using") ] + TEXT("\n");
		PotComp->CreateElementSucced(result[TEXT("Result")], s);
		UE_LOG(LogTemp,Warning,TEXT("String Length: %s"),*result[TEXT("Result")]);
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

void AKMK_HttpActorWithAI::ParsecNewInfo(FString& respon, bool isInit)
{
	TArray<FString> SectionName = { TEXT("1. 기본 정보"), TEXT("2. 특성"), TEXT("3. 용도"), TEXT("4. 흥미로운 사실") };
	if ( respon.IsEmpty() )
	{
		HttpUI->MakeChatText(respon, 1);
		return;
	}
	if(!isInit )
	{
		TMap<FString,TMap<FString,FString>> result = UKMK_JsonParseLib::ChatBotParsec(respon,SectionName);
		if(result.IsEmpty()) return;
		for ( int i = 0; i < SectionName.Num(); i++ )
		{
			FString arr;
			int count = 0;
			const TMap<FString,FString>& SupMap = result[ SectionName[ i ] ];
			for ( const TPair<FString,FString>& Pair : SupMap )
			{
				FString t = Pair.Key + TEXT("\n") + Pair.Value + TEXT("\n\n");
				if ( i == 0 && count == 0 )
				{
					arr += result[ TEXT("Header") ][ TEXT("Content") ] + TEXT("\n\n") + t;
					count++;
				}
				else arr += t;
			}
			// if(i == SectionName.Num() - 1 ) arr += result[TEXT("Footer")][ TEXT("Content") ];
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

