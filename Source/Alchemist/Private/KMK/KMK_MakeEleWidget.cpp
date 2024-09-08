// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_MakeEleWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UKMK_MakeEleWidget::SetSlotChild(UWidget* wid,class AKMK_HttpActorWithAI* httpComp)
{
    http = httpComp;
    SetVisibility(ESlateVisibility::Visible);
    auto* childSlot = DetailHorizontalBox->AddChildToHorizontalBox(wid);
    if ( childSlot )
    {
        childSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
    }
}
