// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SYH_PlayerAnim.generated.h"


/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_PlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly)
	class ASYH_Player* me;
	UPROPERTY(EditDefaultsOnly)
	class ASYH_MultiPlayer* Multi_me;
	UPROPERTY()
	class APlayerController* player;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float horizontal; // direction
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float vertical; // speed

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float multi_horizontal; // direction
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float multi_vertical; // speed
	
	//UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	//bool bIsCamera = false;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	bool bIsPlayCameraAnim = true;
	UFUNCTION()
	void AnimNotify_CameraEnd();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UUserWidget> CameraWidgetClass;

	class UUserWidget* CameraWidget;

	// 몸 굽히기
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float PitchAngle;
};
