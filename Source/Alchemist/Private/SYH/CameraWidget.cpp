// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/CameraWidget.h"

#include "Components/ProgressBar.h"
#include "Components/Slider.h"

void UCameraWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ZoomSlider)
	{
		ZoomSlider->OnValueChanged.AddDynamic(this, &UCameraWidget::OnSliderValueChanged);
	}
}


void UCameraWidget::OnSliderValueChanged(float value)
{
	const float MinFOV = 30.0f; 
	const float MaxFOV = 90.0f; 

	// lerp를 사용하여  slider의 값이 변함에 따라 선형적으로 fov값이 변하도록 함
	float NewFOV = FMath::Lerp(MaxFOV, MinFOV, value);

	if(APlayerController* player = GetOwningPlayer())
	{
		if(APlayerCameraManager* CameraManager = player->PlayerCameraManager)
		{
			CameraManager->SetFOV(NewFOV);
		}
	}
}
