// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_DeskComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "SYH/SYH_MultiPlayer.h"

// Sets default values for this component's properties
UKMK_DeskComponent::UKMK_DeskComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UKMK_DeskComponent::BeginPlay()
{
	Super::BeginPlay();	
	// 책상을 비추는 카메라를 찾고 비활성화 시키는 함수
	player = GetWorld()->GetFirstPlayerController();
	check(player);
	if ( player)
	{
		AActor* viewTarget = player->GetViewTarget();
		PlayerViewCam = Cast<ACameraActor>(viewTarget);
		FindDeskCam();
	}
}


// Called every frame
void UKMK_DeskComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// 변경된 카메라 활성 시키는 함수
	// ChangeMyCamera(bActive);
	if(bActive)player->SetShowMouseCursor(true);
	else player->SetShowMouseCursor(false);
}

void UKMK_DeskComponent::FindDeskCam()
{
	// 카메라 actor들을 담을 변수
	TArray<AActor*> FoundActors;
	// 레벨에서 카메라를 가진 엑터를 모두 찾기
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);
	// 탐색
	for (AActor* Actor : FoundActors)
	{
		// 카메라 엑터가 있는 경우 ACameraActor로 변환
		auto* CameraActor = Cast<ACameraActor>(Actor);
		// UCameraComponent 뽑아내기
		auto* CamComp = CameraActor->FindComponentByClass<UCameraComponent>();
		// 데스크라면
		if(CamComp)
		{
			if (Actor->ActorHasTag(TEXT("Desk")))
			{
				// DeskCameraComponent에 변수값을 넣어주고
				if (!DeskCameraComponent)
				{
					DeskCameraComponent = CamComp;
					// 비활성화
					DeskCameraComponent->SetActive(false);
					if(PlayerViewCam)PlayerViewCam->GetComponentByClass<UCameraComponent>()->SetActive(true);
				}
			}
		}
	}
}
