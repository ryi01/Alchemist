// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/MissionWidget.h"
#include "Components/TextBlock.h"

void UMissionWidget::SetMissionText(int32 index)
{
    num = index;
    if ( missionArray.IsEmpty() == false )
    {
        UE_LOG(LogTemp,Warning,TEXT("Setting text: %s"),*missionArray[ num ]);
        MissionText->SetText(FText::FromString(missionArray[ num ]));
    }
}
