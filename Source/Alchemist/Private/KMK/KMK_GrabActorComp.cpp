// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_GrabActorComp.h"
#include "Components/BoxComponent.h"
#include "KMK/KMK_SingleIntaraction.h"
#include "KMK/KMK_ElementGameActor.h"
#include "Components/WidgetComponent.h"
#include "KMK/KMK_MakeEleWidget.h"
#include "../CHJ/Guide_GameInstance.h"
#include "KMK/MissionWidget.h"
#include "KMK/KMK_PlayerMouse.h"
#include "KMK/PlayerInteractionComponent.h"

// Sets default values for this component's properties
UKMK_GrabActorComp::UKMK_GrabActorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UKMK_GrabActorComp::BeginPlay()
{
	Super::BeginPlay();

	auto* box = GetOwner()->FindComponentByClass<UBoxComponent>();
	check(box);
	if ( box )
	{
		box->OnComponentBeginOverlap.AddDynamic(this, &UKMK_GrabActorComp::BeginOverlap);
	}

}


// Called every frame
void UKMK_GrabActorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UKMK_GrabActorComp::CreateElementSucced(FString tagName, const FString& text)
{
	if ( !isCreate )
	{
		isCreate = true;
		newActor = GetWorld()->SpawnActor<AActor>(elementFact,GetOwner()->GetTargetLocation(),FRotator::ZeroRotator);
		newActor->Tags.Add(FName(*tagName));
		auto* newText = newActor->GetComponentByClass<UKMK_ElementGameActor>();
		if ( newText )
		{
			newText->OnCreateWidget(false);
			newText->SetTextWidget(text);
		}
		// 중복 확인
		if ( player != nullptr )
		{
			if ( !player->collectionTag.Contains(tagName) )
			{
				player->collectionTag.Add(*tagName);
				if(missionWidget != nullptr) MissionComplete(tagName,count,newActor);
				
				count++;
			}
			else
			{
				FTimerHandle handle;
				GetWorld()->GetTimerManager().SetTimer(handle,FTimerDelegate::CreateLambda([this]()
					{

						newActor->Destroy();

					}),5,false);
					
			}
		}

	}
}

void UKMK_GrabActorComp::CreateElementFailed()
{
	if ( httpActor != nullptr && cnt < 1 )
	{
		httpActor->ReqRecommandEle(json);
		cnt++;
	}
}

void UKMK_GrabActorComp::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 액터 중에 tag = element인것만 넣기
	if ( OtherActor->ActorHasTag(TEXT("element")) )
	{
		FString checkTagName = OtherActor->Tags[1].ToString();
		if(checkTagName == "" ) return;
		if ( ElementArray.Contains(checkTagName) ) ElementArray[ checkTagName ]++;
		else ElementArray.Add(checkTagName,1);

		OtherActor->Destroy();
	}

}

// 엘리먼트가 모아지면 태그 여부 확인하고 BP를 생성함
void UKMK_GrabActorComp::CreateElementBP(FString tag)
{
	if(player == nullptr ) return;	
	if(collectionEle >= elementPosArray.Num() || createEletags.Contains(tag) ||( player != nullptr && player->collectionTag.Contains(tag))) return;
	collectionEle++;
	auto eleActor = GetWorld()->SpawnActor<AActor>(elementFactory,elementPosArray[ collectionEle ],FRotator::ZeroRotator);
	eleActor->Tags.Add(FName(*tag));
	createEletags.Add(tag);
	eleActor->SetActorHiddenInGame(false);

	auto* mesh = eleActor->FindComponentByClass<UStaticMeshComponent>();
	if ( mesh )
	{
		if ( eleMeshMap.Num() > 0 && eleMeshMap.Contains(tag))
		{
			mesh->SetStaticMesh(eleMeshMap[tag]);
			mesh->SetWorldRotation(FRotator(0, -90, 0));
		}
	}
}

// 태그 검사하기
void UKMK_GrabActorComp::MissionComplete(const FString& missionEleTag, int32 num, AActor* actor)
{
	if(missionTag.IsEmpty()) return;
	if ( missionTag.Contains(missionEleTag) )
	{

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle,FTimerDelegate::CreateLambda([this, actor, num]()
			{
				player->textNum++;
				actor->Destroy();

			}), 5, false);
		

	}
	else
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle,FTimerDelegate::CreateLambda([this,actor,num]()
			{
				actor->Destroy();

			}),5,false);

	}

}

