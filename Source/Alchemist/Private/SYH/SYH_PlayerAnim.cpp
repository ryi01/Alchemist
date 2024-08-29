// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_PlayerAnim.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "SYH/SYH_Player.h"

void USYH_PlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	me = Cast<ASYH_Player>(TryGetPawnOwner());
}

void USYH_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if ( me == nullptr )
	{
		return;
	}
	FVector velocity = me->GetVelocity();
	FVector right = me->GetActorRightVector();
	FVector forward = me->GetActorForwardVector();

	horizontal = FVector::DotProduct(velocity, right);
	vertical = FVector::DotProduct(velocity, forward);
}

// 카메라를 드는 애니메이션이 끝나면 시점을 변경하고 UI를 띄우고 싶다.
void USYH_PlayerAnim::AnimNotify_CameraEnd()
{
	bIsCamera = false;
	// camera를 1인칭 시점으로 변경한다.
	me->CameraCompThird->SetActive(false);
	me->CameraCompFirst->SetActive(true);
	// 시점을 변경한 후 사진을 찍는 듯한 UI를 띄운다.
	if(CameraWidgetClass)
	{
		player = Cast<APlayerController>(me->Controller);
		CameraWidget = CreateWidget<UUserWidget>(player, CameraWidgetClass);
		if(CameraWidget)
		{
			CameraWidget->AddToViewport();
		}
		// 시점을 바꾼 상태에서는 e키를 누르지 못하게 bool값으로 조절
		bIsPlayCameraAnim = false;
	}
}
