// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_PlayerMouse.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "KMK/KMK_HttpActorWithAI.h"
#include "KMK/KMK_GrabActorComp.h"
#include "Kismet/GameplayStatics.h"
#include "KMK/KMK_DeskComponent.h"

// Sets default values for this component's properties
UKMK_PlayerMouse::UKMK_PlayerMouse()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_PlayerMouse::BeginPlay()
{
	Super::BeginPlay();
	me = CastChecked<APlayerController>(GetWorld()->GetFirstPlayerController());
	handle = GetOwner()->GetComponentByClass<UPhysicsHandleComponent>();
}


// Called every frame
void UKMK_PlayerMouse::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(me == nullptr ) return;
	if( !IsRay ) OnMyPutComp(outHitComp);
	else OnMyGrabComp();

	if ( outHitComp == nullptr ) return;

	TArray<FVector> pos = GetMouseWorldDirection();

	handle->SetTargetLocation(pos[0] + pos[1] * distance);

}
// 원소 조합시에 사용되는 함수
void UKMK_PlayerMouse::OnMyGrabComp()
{
	if(count > 0) return;

	FHitResult outHit;
	mousePos = GetMouseWorldDirection();
	ECollisionChannel channel = ECollisionChannel::ECC_GameTraceChannel1;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, mousePos[0], mousePos[0] + mousePos[1] * RayDis, channel, params);
	if ( bHit )
	{
		auto* hitActor = outHit.GetActor();
		if ( hitActor->ActorHasTag(TEXT("Pot")) )
		{
			auto* potComp = hitActor->GetComponentByClass<UKMK_GrabActorComp>();
			TArray<AActor*> arrActor;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(),AKMK_HttpActorWithAI::StaticClass(),arrActor);
			HttpActor = arrActor[ 0 ];
			if ( HttpActor == nullptr ) return;
			auto* httpComp = CastChecked<AKMK_HttpActorWithAI>(HttpActor);
			if ( httpComp )
			{
				TMap<FString,FString> ConvertedMap;
				for ( const TPair<FString,int>& pair : potComp->ElementArray )
				{
					ConvertedMap.Add(pair.Key,FString::FromInt(pair.Value));
				}
				httpComp->ReqElement(ConvertedMap);
				potComp->ElementArray.Empty();
			}
			return;
		}
		if ( hitActor->ActorHasTag(TEXT("Desk")))
		{
			auto* desk = hitActor->GetComponentByClass<UKMK_DeskComponent>();
			if ( desk )
			{
				desk->ChangeMyCamera(true);
			}
			return;
		}

		if ( hitActor )
		{
			// 원본 액터인지 복사본인지 판별
			if ( hitActor->ActorHasTag("Copy") )
			{
				// 이미 복사된 액터이므로 스킵하거나 다른 처리를 할 수 있음
				outHitComp = outHit.GetComponent();
				outHitComp->SetSimulatePhysics(true);
				handle->GrabComponentAtLocation(outHitComp, NAME_None, outHit.ImpactPoint);
				return;
			}
			else
			{
				CopyNewActor(hitActor, outHit.ImpactPoint);
			}
		}

	}


}

void UKMK_PlayerMouse::OnMyPutComp(class UPrimitiveComponent* comp)
{
	if(comp == nullptr) return;
	count = 0;
	comp->SetSimulatePhysics(false);
	handle->ReleaseComponent();
}

void UKMK_PlayerMouse::CopyNewActor(AActor* hitActor, FVector grabPos)
{
	auto* ActorClass = hitActor->GetClass();
	AActor* newActor = GetWorld()->SpawnActor<AActor>(ActorClass, hitActor->GetActorLocation(), FRotator::ZeroRotator);
	count++;
	if ( newActor )
	{
		if(hitActor->Tags[1].IsValid())newActor->Tags.Add(hitActor->Tags[1]);
		newActor->Tags.Add("Copy");
		if ( handle )
		{
			auto* mat = newActor->GetComponentByClass<UStaticMeshComponent>();
			mat->SetMaterial(0, hitActor->FindComponentByClass<UStaticMeshComponent>()->GetMaterial(0));
			outHitComp = newActor->FindComponentByClass<UPrimitiveComponent>();
			check(outHitComp);
			if ( outHitComp )
			{
				outHitComp->SetSimulatePhysics(true);
				handle->GrabComponentAtLocation(outHitComp, NAME_None, grabPos);
			}
		}
	}
}

TArray<FVector> UKMK_PlayerMouse::GetMouseWorldDirection()
{
	TArray<FVector> arr;
	FVector WorldLocation, WorldDirection;

	me->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	arr.Add(WorldLocation);
	arr.Add(WorldDirection);
	return arr;
}



