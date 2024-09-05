// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_PlayerAnim.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "SYH/SYH_MultiPlayer.h"
#include "SYH/SYH_Player.h"

void USYH_PlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

}

void USYH_PlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	me = Cast<ASYH_Player>(TryGetPawnOwner());
	Multi_me = Cast<ASYH_MultiPlayer>(TryGetPawnOwner());
	if ( me != nullptr )
	{
		FVector velocity = me->GetVelocity();
		FVector right = me->GetActorRightVector();
		FVector forward = me->GetActorForwardVector();

		horizontal = FVector::DotProduct(velocity, right);
		vertical = FVector::DotProduct(velocity, forward);
	}


	// multi ver

	if (Multi_me == nullptr)
	{
		return ;
	}
	FVector multi_velocity = Multi_me->GetVelocity();
	FVector multi_right = Multi_me->GetActorRightVector();
	FVector multi_forward = Multi_me->GetActorForwardVector();

	multi_horizontal = FVector::DotProduct(multi_velocity, multi_right);
	multi_vertical = FVector::DotProduct(multi_velocity, multi_forward);

	PitchAngle = -Multi_me->GetBaseAimRotation().Pitch;
}

//카메라를 드는 애니메이션이 끝나면 시점을 변경하고 UI를 띄우고 싶다.
void USYH_PlayerAnim::AnimNotify_CameraEnd()
{
	//Multi_me->bIsCamera = false;
	// camera를 1인칭 시점으로 변경한다.
	Multi_me->CameraCompThird->SetActive(false);
	Multi_me->CameraCompFirst->SetActive(true);
	// 1인칭일 때 몸이 보이기 때문에 안보이게 설정
	if (Multi_me->GetMesh())
	{
		Multi_me->GetMesh()->SetOwnerNoSee(true);
	}
	// 시점을 변경한 후 사진을 찍는 듯한 UI를 띄운다
	if(CameraWidgetClass)
	{
		if(Multi_me->Controller != nullptr) player = CastChecked<APlayerController>(Multi_me->Controller);
		if(player && player->IsLocalPlayerController())
		{
			CameraWidget = CreateWidget<UUserWidget>(player, CameraWidgetClass);
			if(CameraWidget)
			{
				CameraWidget->AddToViewport(0);
			}
			// 시점을 바꾼 상태에서는 e키를 누르지 못하게 bool값으로 조절
			bIsPlayCameraAnim = false;
		}
	}
}
