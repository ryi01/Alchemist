// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/MissionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/HorizontalBox.h"
#include "Components/SizeBox.h"

void UMissionWidget::SetMissionText(int32 index)
{
    num = index;
    if ( missionArray.IsEmpty() == false )
    {
        MissionImage->SetBrushFromTexture(missionArray[ num ]);
    }
}

void UMissionWidget::SetClickImageVisi(ESlateVisibility visible)
{
    Click1Image->SetVisibility(visible);
    ClickText->SetVisibility(visible);
}

void UMissionWidget::SetTimerEvent(int32 t)
{
    TimeHorizonBox->SetVisibility(ESlateVisibility::Visible);
    TimeBox->SetVisibility(ESlateVisibility::Visible);

    SecText->SetText(FText::AsNumber(t));
}

void UMissionWidget::HiddenTime()
{
    TimeHorizonBox->SetVisibility(ESlateVisibility::Hidden);
    TimeBox->SetVisibility(ESlateVisibility::Hidden);
}
