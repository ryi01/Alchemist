// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/CameraWidget.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Input/Reply.h"
#include "SYH/SYH_MultiPlayer.h"
#include "SYH/SYH_QuizSelect.h"
#include "SYH/SYH_QuizWaitWidget.h"


void UCameraWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZoomSlider)
	{
		ZoomSlider->OnValueChanged.AddDynamic(this, &UCameraWidget::OnSliderValueChanged);
	}
	me = CastChecked<ASYH_MultiPlayer>(GetOwningPlayerPawn());
}
void UCameraWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	playercontroller = Cast<APlayerController>(GetOwningPlayer());
	if(playercontroller)
	{
		playercontroller->SetShowMouseCursor(true);
	}
	UpdateTextVisibility();
}

void UCameraWidget::OnSliderValueChanged(float value)
{
	const float MinFOV = 30.0f; 
	const float MaxFOV = 90.0f; 

	// lerp를 사용하여  slider의 값이 변함에 따라 선형적으로 fov값이 변하도록 함
	float NewFOV = FMath::Lerp(MaxFOV, MinFOV, value);

	if(playercontroller && playercontroller == GetOwningPlayer() )
	{
		if(APlayerCameraManager* CameraManager = playercontroller->PlayerCameraManager)
		{
			CameraManager->SetFOV(NewFOV);
		}
	}
}

FReply UCameraWidget::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if(MouseEvent.GetEffectingButton() == EKeys::RightMouseButton && IsTaggedActorInView())
	{
		if(camera)
		{
			PlayAnimation(camera);
			UGameplayStatics::PlaySound2D(GetWorld(),CameraSound);
		}
		if(me)
		{
			me->ObjectDetect();
		}
	}
	return Super::NativeOnMouseButtonDown(MyGeometry, MouseEvent);
}

bool UCameraWidget::IsTaggedActorInView()
{
	if(!CameraImage)
	{
		return false;
	}
	TArray<AActor*> TaggedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),"CameraObject",TaggedActors);

	if(!playercontroller) return false;

	for (AActor* actor : TaggedActors)
	{
		if (actor)
		{
			FVector2D ScreenPos;
			FVector ActorPos = actor->GetActorLocation();

			// 스크린 좌표로 변환
			if (playercontroller->ProjectWorldLocationToScreen(ActorPos, ScreenPos))
			{
				// 뷰포트 크기를 가져옴
				FVector2D ViewportSize;
				GEngine->GameViewport->GetViewportSize(ViewportSize);

				// 화면 내에 Actor가 있는지 확인 (스크린 좌표가 뷰포트 내에 있는지)
				if (ScreenPos.X >= 0 && ScreenPos.X <= ViewportSize.X &&
					ScreenPos.Y >= 0 && ScreenPos.Y <= ViewportSize.Y)
				{
					return true;  // 감지 성공
				}
			}
		}
	}
	return false;
}

void UCameraWidget::UpdateTextVisibility()
{
	if(IsTaggedActorInView())
	{
		Yes->SetVisibility(ESlateVisibility::Visible);
		No->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		Yes->SetVisibility(ESlateVisibility::Hidden);
		No->SetVisibility(ESlateVisibility::Visible);
	}
}
