// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/PlayerInteractionComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputTriggers.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "SYH/SYH_MultiPlayer.h"
#include "KMK/KMK_DeskComponent.h"
#include "KMK/KMK_PlayerMouse.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "KMK/KMK_SingleIntaraction.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerInteractionComponent::UPlayerInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
	me = Cast<ASYH_MultiPlayer>(GetOwner());
	if ( me->IsLocallyControlled() )
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(),TEXT("Desk"),FoundActors);

		for ( AActor* Actor : FoundActors )
		{
			if ( Actor )
			{
				auto desk = Actor->FindComponentByClass<UKMK_DeskComponent>();
				deskMouse = Actor->FindComponentByClass<UKMK_PlayerMouse>();
				if ( desk )
				{
					desk->FindDeskCam(Cast<APlayerController>(me->Controller));
				}
				if ( deskMouse )
				{
					deskMouse->me = Cast<APlayerController>(me->Controller);
				}
			}
		}
	}
}


// Called every frame
void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ( me->IsLocallyControlled() )
	{
		FHitResult OutHit;
		FVector start = me->CameraCompFirst->GetComponentLocation();
		FVector end = start + me->CameraCompFirst->GetForwardVector() * 800;
		float Radius = 60.f;
		ECollisionChannel TraceChannel = ECC_Visibility;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(me);

		// 카메라 앞방향으로 1KM 선을 쏘고 싶다
		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,start,end, TraceChannel,Params);

		if ( HitResult.GetActor() != nullptr && HitResult.bBlockingHit )
		{
			OnMyCheckActor();
		}
		else
		{
			DrawDebugLine(GetWorld(),start,end, FColor::Blue);
		}
	}

}

void UPlayerInteractionComponent::SetupInputBinding(class UEnhancedInputComponent* input)
{
	if ( input )
	{
		input->BindAction(IA_Interaction,ETriggerEvent::Started,this,&UPlayerInteractionComponent::OnMyActionInteraction);
	}
}

void UPlayerInteractionComponent::OnMyActionInteraction(const FInputActionValue& Value)
{
	if ( HitResult.bBlockingHit )
	{
		AActor* HitActor = HitResult.GetActor();
		if ( HitActor )
		{
			auto* desk = HitActor->FindComponentByClass<UKMK_DeskComponent>();
			auto* mouse = HitActor->FindComponentByClass<UKMK_PlayerMouse>();
			if ( HitActor->ActorHasTag(TEXT("Desk")) )
			{
				if ( desk )
				{
					DeskActor = HitActor;
					mouse->isDesk = true;
					mouse->handle = me->FindComponentByClass<UPhysicsHandleComponent>();
					me->PlayerController->SetShowMouseCursor(true);
					
					desk->ChangeMyCamera(true);
				}
				return;
			}
			count = 0;
			auto* actorClass = HitActor->FindComponentByClass
			<UKMK_SingleIntaraction>();
			if ( actorClass )
			{
				actorClass->CreatePlayerWidget(true,0,me->PlayerController);
				me->PlayerController->SetPause(true);
				
			}
		}
	}
}
void UPlayerInteractionComponent::OnMyCheckActor()
{
	AActor* HitActor = HitResult.GetActor();
	if ( interActor != nullptr && HitActor != interActor->GetOwner() )
	{
		if ( interActor->bMouseOnActor )
		{
			interActor->OnCreateNameWidget(false);
		}
	}
	if ( HitActor )
	{
		interActor = HitActor->GetComponentByClass<UKMK_SingleIntaraction>();
		if ( interActor )
		{
			interActor->OnCreateNameWidget(true);
		}

	}
}
