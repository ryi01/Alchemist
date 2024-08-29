// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "KMK_DeskComponent.generated.h"

// å�� �� Ŭ���� ������Ʈ�� �������� ���� ����
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

	// å�� ���� ��� �ִ� ī�޶�
	UPROPERTY()
	class UCameraComponent* DeskCameraComponent;
	UPROPERTY()
	class UCameraComponent* PlayerCamera;
	UPROPERTY()
	class APlayerController* player;
	UPROPERTY(EditAnywhere)
	bool bActive = false;
	// Ŭ���Ǹ� Ȱ��ȭ�� �Լ� = �ζ��� �Լ����
	UFUNCTION()
	void ChangeMyCamera(bool OnClick)
	{
		// å�� Ŭ���� ī�޶� ����
		if (OnClick)
		{
			// ����ũ ī�޶� Ȱ��ȭ
			player->SetViewTarget(DeskCameraComponent->GetOwner());
		}
		else
		{
			// �÷��̾� ī�޶� Ȱ��ȭ
			player->SetViewTarget(PlayerCamera->GetOwner());
		}
	}
	// ī�޶� ã�� �Լ�
	void FindDeskCam();

};
