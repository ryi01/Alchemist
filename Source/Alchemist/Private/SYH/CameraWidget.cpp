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

void UCameraWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateTextVisibility();
}


void UCameraWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZoomSlider)
	{
		ZoomSlider->OnValueChanged.AddDynamic(this, &UCameraWidget::OnSliderValueChanged);
	}
	playercontroller = Cast<APlayerController>(GetOwningPlayer());
	me = CastChecked<ASYH_MultiPlayer>(GetOwningPlayerPawn());
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
	if(MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if(camera)
		{
			PlayAnimation(camera);
		}
		me->ObjectDetect();
		return FReply::Handled();
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

	for(AActor* actor : TaggedActors)
	{
		if(actor)
		{
			FVector2D ScreenPos;
			FVector ActorPos = actor->GetActorLocation();
			if(playercontroller->ProjectWorldLocationToScreen(ActorPos,ScreenPos))
			{
				FVector2D CameraPos = CameraImage->GetCachedGeometry().GetAbsolutePosition();
				FVector2D CameraSize = CameraImage->GetCachedGeometry().GetLocalSize();

				if(ScreenPos.X >= CameraPos.X && ScreenPos.X <= CameraPos.X + CameraSize.X &&
					ScreenPos.Y >= CameraPos.Y && ScreenPos.Y <= CameraPos.Y + CameraSize.Y)
				{
					return true;
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
