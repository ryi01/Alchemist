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
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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
			newText->OnCreateWidget(true);
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
		// ( X = 7250.000000,Y = 52745.403291,Z = -15495.000000 )
		auto eleActor = GetWorld()->SpawnActor<AActor>(noCollsionFact,createPos[createNum],FRotator::ZeroRotator);
		eleActor->SetActorHiddenInGame(false);
		actorArray.Add(eleActor);

		auto* mesh = eleActor->FindComponentByClass<UStaticMeshComponent>();
		if ( mesh )
		{
			if ( eleMeshMap.Num() > 0 && eleMeshMap.Contains(checkTagName) )
			{
				mesh->SetStaticMesh(eleMeshMap[ checkTagName ]);
				mesh->SetWorldRotation(FRotator(0,-90,0));
			}
		}
		createNum++;
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
	UGameplayStatics::PlaySound2D(GetWorld(),sounds[ 0 ]);
	// 파티클 시스템 스폰
	UParticleSystemComponent* ParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),  // 파티클을 스폰할 월드
		particle[0],  // 재생할 파티클 시스템
		GetOwner()->GetActorLocation()+FVector(300, 0, 0),  // 파티클이 생성될 위치
		FRotator::ZeroRotator  // 파티클의 회전
	);

	if ( ParticleComponent )
	{
		ParticleComponent->ActivateSystem();  // 파티클 재생
	}
	if ( missionTag.Contains(missionEleTag) )
	{

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle,FTimerDelegate::CreateLambda([this, actor, num]()
			{
				player->textNum += 1;
				player->missionWidget->SetMissionText(player->textNum);
				actor->Destroy();

			}), 5, false);
		
		for ( int i = 0; i < actorArray.Num(); i++ )
		{
			actorArray[ i ]->Destroy();
		}
		actorArray.Empty();
		isCreate = false;
		ElementArray.Empty();
		createNum = 0;
	}
	else
	{
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle,FTimerDelegate::CreateLambda([this,actor,num]()
			{
				actor->Destroy();
			}),5,false);

		for ( int i = 0; i < actorArray.Num(); i++ )
		{
			actorArray[ i ]->Destroy();
		}
		actorArray.Empty();
		isCreate = false;
		ElementArray.Empty();
		createNum = 0;
	}

}

