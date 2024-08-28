// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_HttpActorWithAI.h"
#include "HttpModule.h"
#include "HttpFwd.h"

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
	
}

// Called every frame
void AKMK_HttpActorWithAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKMK_HttpActorWithAI::ReqPostAI(FString json)
{
	// HTTP ��� ����
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> req = httpModule.CreateRequest();
	// ��û�� ������ ����
	req->SetURL(Aurl);
	req->SetVerb(TEXT("POST"));
	req->SetHeader(TEXT("content-type"), TEXT("application/json"));
	req->SetContentAsString(json);

	// ������� �Լ��� ����
	req->OnProcessRequestComplete().BindUObject(this, &AKMK_HttpActorWithAI::OnResPostAi);
	// ������ ��û
	req->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("Conncting with AI"));
}

void AKMK_HttpActorWithAI::OnResPostAi(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (bConnectedSuccessfully)
	{
		// ����
		FString result = Response->GetContentAsString();
		UE_LOG(LogTemp, Warning, TEXT("OnResNewBookInfo Succed"));
	}
	else 
	{
		// ����
		UE_LOG(LogTemp, Warning, TEXT("OnResNewBookInfo Failed..."));
	}
}

