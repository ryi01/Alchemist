// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_PlayerMouse.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "KMK/KMK_HttpActorWithAI.h"
#include "KMK/KMK_GrabActorComp.h"
#include "Kismet/GameplayStatics.h"
#include "KMK/KMK_DeskComponent.h"
#include "KMK/KMK_StudyWidget.h"
#include "Blueprint/UserWidget.h"
#include "KMK/KMK_ElementGameActor.h"
#include "KMK/KMK_TextWidget.h"
#include "KMK/KMK_SingleIntaraction.h"

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
	TArray<AActor*> arrActor;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AKMK_HttpActorWithAI::StaticClass(),arrActor);
	if ( arrActor[ 0 ] )HttpActor = arrActor[ 0 ];
	httpComp = CastChecked<AKMK_HttpActorWithAI>(HttpActor);
	me = CastChecked<APlayerController>(GetWorld()->GetFirstPlayerController());
	for ( int i = 0; i < 5; i++ )
	{
		elementPos.Add(FVector(1890, -1970 + 90 * i, 80));
	}
	
}


// Called every frame
void UKMK_PlayerMouse::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if ( me == nullptr || httpComp == nullptr || httpComp->isWidgetOn ) return;

	if( !IsRay ) OnMyPutComp(outHitComp);
	else OnMyGrabComp();

	//FHitResult HitResult;
	//me->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);
	//if ( HitResult.GetActor() != nullptr && HitResult.bBlockingHit && HitResult.GetActor()->ActorHasTag(TEXT("NewEle")))
	//{
	//	OnMyCheckActor(HitResult);
	//}
	if ( outHitComp == nullptr ) return;

	TArray<FVector> pos = GetMouseWorldDirection();

	handle->SetTargetLocation(pos[0] + pos[1] * distance);

}
// 원소 조합시에 사용되는 함수
void UKMK_PlayerMouse::OnMyGrabComp()
{
	if(count > 0) return;
	if ( !isDesk ) return;
	FHitResult outHit;
	mousePos = GetMouseWorldDirection();
	ECollisionChannel channel = ECollisionChannel::ECC_GameTraceChannel1;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	bool bHit = GetWorld()->LineTraceSingleByChannel(outHit, mousePos[0], mousePos[0] + mousePos[1] * RayDis, channel, params);
	if ( bHit )
	{
		auto* hitActor = outHit.GetActor();
		if ( hitActor->ActorHasTag("NewEle") && outHitComp == nullptr )
		{
			// 엑터에 컴포넌트에 정보 업데이트
			elementActor = hitActor->GetComponentByClass<UKMK_ElementGameActor>();
			if(elementActor == nullptr ) return;
			if ( !elementActor->isOnWidget && eleCount < elementPos.Num())
			{
				elementActor->OnCreateWidget(true);
				// Actor의 Comp에 정보 업데이트
				elementActor->ChangeMyPos(elementPos[ eleCount ]);
				eleCount++;
				potComp->isCreate = false;
				potComp->ElementArray.Empty();
				GEngine->AddOnScreenDebugMessage(1, 1, FColor::Green, FString::Printf(TEXT("%d"), eleCount));
			}
			return;
		}
		if ( hitActor->ActorHasTag(TEXT("Pot")) && outHitComp == nullptr )
		{
			potComp = hitActor->GetComponentByClass<UKMK_GrabActorComp>();
			if(potComp->ElementArray.IsEmpty() ) return;

			FString result = TEXT("");
			
			if ( httpComp )
			{
				for ( const TPair<FString,int>& pair : potComp->ElementArray )
				{
					FString value = "";
					if(pair.Value != 1 ) value = FString::FromInt(pair.Value);
					result += pair.Key + value;
				}
				httpComp->ReqElement(result, potComp);
				potComp->SetHttpActor(httpComp,result);
			}
			return;
		}

		if ( hitActor )
		{
			// 원본 액터인지 복사본인지 판별
			if ( hitActor->ActorHasTag(TEXT("Copy") ))
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
	outHitComp = nullptr;
}

void UKMK_PlayerMouse::CopyNewActor(AActor* hitActor, FVector grabPos)
{
	auto* ActorClass = hitActor->GetClass();
	AActor* newActor = GetWorld()->SpawnActor<AActor>(ActorClass, hitActor->GetActorLocation(), FRotator::ZeroRotator);
	count++;
	if ( newActor )
	{
		if( hitActor->Tags[ 0 ] != TEXT("Pot") && hitActor->Tags[1].IsValid())newActor->Tags.Add(hitActor->Tags[1]);
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

void UKMK_PlayerMouse::OnMyCheckActor(FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();
	if ( interActor != nullptr && HitActor != interActor->GetOwner() )
	{
		if ( interActor != nullptr && HitActor != interActor->GetOwner() )
		{
			//interActor->OnCreateWidget(false);
		}
	}
	if ( HitActor )
	{
		interActor = HitActor->GetComponentByClass<UKMK_ElementGameActor>();
		if ( interActor )
		{
			//interActor->OnCreateWidget(true);
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
