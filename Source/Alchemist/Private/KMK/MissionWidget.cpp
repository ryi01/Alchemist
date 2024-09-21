// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/MissionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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
}
