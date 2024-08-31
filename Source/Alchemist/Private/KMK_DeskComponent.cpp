// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_DeskComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

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
	// å���� ���ߴ� ī�޶� ã�� ��Ȱ��ȭ ��Ű�� �Լ�
	FindDeskCam();
	player = GetWorld()->GetFirstPlayerController();
	if (player)
	{
		auto* playerController = Cast<ACharacter>(player->GetPawn());
		if (playerController)
		{
			PlayerCamera = playerController->FindComponentByClass<UCameraComponent>();
		}
	}
}


// Called every frame
void UKMK_DeskComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ����� ī�޶� Ȱ�� ��Ű�� �Լ�
	ChangeMyCamera(bActive);
}

void UKMK_DeskComponent::FindDeskCam()
{
	// ī�޶� actor���� ���� ����
	TArray<AActor*> FoundActors;
	// �������� ī�޶� ���� ���͸� ��� ã��
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);
	// Ž��
	for (AActor* Actor : FoundActors)
	{
		// ī�޶� ���Ͱ� �ִ� ��� ACameraActor�� ��ȯ
		auto* CameraActor = Cast<ACameraActor>(Actor);
		// UCameraComponent �̾Ƴ���
		auto* CamComp = CameraActor->GetComponentByClass<UCameraComponent>();
		// ����ũ���
		if(CamComp)
		{
			if (Actor->ActorHasTag(TEXT("Desk")))
			{
				// DeskCameraComponent�� �������� �־��ְ�
				if (!DeskCameraComponent)
				{
					DeskCameraComponent = CamComp;
					// ��Ȱ��ȭ
					DeskCameraComponent->SetActive(false);
				}
			}
		}
	}
}
