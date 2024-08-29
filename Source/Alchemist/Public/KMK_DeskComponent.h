// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "KMK_DeskComponent.generated.h"

// 책상 위 클릭시 오브젝트가 펼쳐지게 만들 예정
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UKMK_DeskComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_DeskComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 책상 위를 찍고 있는 카메라
	UPROPERTY()
	class UCameraComponent* DeskCameraComponent;
	UPROPERTY()
	class UCameraComponent* PlayerCamera;
	UPROPERTY()
	class APlayerController* player;
	UPROPERTY(EditAnywhere)
	bool bActive = false;
	// 클릭되면 활성화될 함수 = 인라인 함수사용
	UFUNCTION()
	void ChangeMyCamera(bool OnClick)
	{
		// 책상 클릭시 카메라 변경
		if (OnClick)
		{
			// 데스크 카메라 활성화
			player->SetViewTarget(DeskCameraComponent->GetOwner());
		}
		else
		{
			// 플레이어 카메라 활성화
			player->SetViewTarget(PlayerCamera->GetOwner());
		}
	}
	// 카메라 찾는 함수
	void FindDeskCam();

};
