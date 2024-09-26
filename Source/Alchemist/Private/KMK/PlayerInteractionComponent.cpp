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
#include "KMK/KMK_GrabActorComp.h"
#include "KMK/MissionWidget.h"
#include "../CHJ/Guide_GameInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KMK/SectionActor.h"

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
				auto deskCam = Actor->FindComponentByClass<UKMK_DeskComponent>();
				deskMouse = Actor->FindComponentByClass<UKMK_PlayerMouse>();
				if ( deskCam )
				{
					deskCam->FindDeskCam(Cast<APlayerController>(me->Controller));
				}
				if ( deskMouse )
				{
					deskMouse->me = Cast<APlayerController>(me->Controller);
				}
			}
		}

		CreateMainWidget();
	}
}


// Called every frame
void UPlayerInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if(desk != nullptr && desk->bActive ) return;
	FHitResult OutHit;
	FVector start = me->CameraCompFirst->GetComponentLocation();
	FVector end = start + me->CameraCompFirst->GetForwardVector() * 1500;
	float Radius = 60.f;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(me);

	// 카메라 앞방향으로 1KM 선을 쏘고 싶다
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult,start,end,TraceChannel,Params);

	if ( HitResult.GetActor() != nullptr && HitResult.bBlockingHit )
	{
		OnMyCheckActor();
	}
	else
	{
		DrawDebugLine(GetWorld(),start,end,FColor::Blue);
	}

	if ( missionWidget && textNum != 0 )
	{
		missionWidget->SetMissionText(textNum);
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
			desk = HitActor->FindComponentByClass<UKMK_DeskComponent>();
			auto* mouse = HitActor->FindComponentByClass<UKMK_PlayerMouse>();
			if ( HitActor->ActorHasTag(TEXT("Desk")) )
			{
				if ( desk && mouse )
				{
					if ( potComp != nullptr )
					{
						potComp->player = this;
						for ( int i = 0; i < me->GameInstance->correctionTag.Num(); i++ )
						{
							potComp->CreateElementBP(me->GameInstance->correctionTag[i]);
						}
					}
					DeskActor = HitActor;
					mouse->isDesk = true;
					mouse->handle = me->FindComponentByClass<UPhysicsHandleComponent>();
					me->SetShowMyMouse(true);
					me->isWidget = true;
					missionWidget->SetClickImageVisi(ESlateVisibility::Visible);
					if ( desk->player == nullptr )
					{
						desk->player = Cast<APlayerController>(me->GetController());
					}
					if ( desk->PlayerCamera == nullptr )
					{
						desk->PlayerCamera = me->CameraCompThird;
					}
					desk->ChangeMyCamera(true);

				}
				return;
			}
			count = 0;
			auto* actorClass = HitActor->FindComponentByClass
				<UKMK_SingleIntaraction>();
			// 못들어가는 곳 들어갈 수 있게 하기
			if ( HitActor->ActorHasTag(TEXT("World")) && collectionTag.IsEmpty() == false )
			{
				if ( collectionTag.Contains(HitActor->Tags[ 1 ]) )
				{
					// actorClass->textWidget->Destroy();
					ASectionActor* comp = Cast<ASectionActor>(HitActor);

					if ( comp )
					{
						// 태그가 포함되면 서버에게 섹션의 isPass를 변경하라고 알려줘야함
                        ServerRPCSectionOpen(comp);
                        comp->comp->SetVisibility(false);
                        // comp->SetCollisionMesh();
						me->ChangeSpeed();
					}
				}
				if ( missionWidget != nullptr )
				{
					textNum++;
				}
				
				return;
			}
			
			if ( actorClass )
			{
				actorClass->CreatePlayerWidget(true,0,me->PlayerController);
				if ( me->PlayerController->IsLocalController() )
				{
					me->PlayerController->SetIgnoreLookInput(true);
					me->PlayerController->SetIgnoreMoveInput(true);
				}
				
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

void UPlayerInteractionComponent::CreateMainWidget()
{
	if ( widget && !bMakeMainWidget)
	{
		missionWidget = Cast<UMissionWidget>(CreateWidget(GetWorld(),widget));
		bMakeMainWidget = true;
		if ( missionWidget )
		{
			missionWidget->AddToViewport();
			missionWidget->SetClickImageVisi(ESlateVisibility::Hidden);
			missionWidget->HiddenTime();
		}
	}
}

void UPlayerInteractionComponent::DeleteMainWidget()
{
	bMakeMainWidget = false;
	if ( missionWidget )
	{
		missionWidget->RemoveFromParent();
	}
}

void UPlayerInteractionComponent::ServerRPCSectionOpen_Implementation(class ASectionActor* target)
{
	if ( me->HasAuthority() )
	{
		MultiRPCSecionOpen(target);
	}
}

void UPlayerInteractionComponent::MultiRPCSecionOpen_Implementation(class ASectionActor* target)
{
	UE_LOG(LogTemp,Warning,TEXT("MultiRPCSecionOpen called"));
	RemoveCollision(target);
}

void UPlayerInteractionComponent::RemoveCollision(class ASectionActor* target)
{
	if ( target && target->comp && me )
	{
		target->comp->IgnoreActorWhenMoving(me,true);
		me->MoveIgnoreActorAdd(target);
	}
}

