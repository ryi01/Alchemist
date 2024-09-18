// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_MakeEleWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Button.h"
#include "KMK/KMK_HttpActorWithAI.h"
#include "KMK/KMK_GrabActorComp.h"

void UKMK_MakeEleWidget::NativeConstruct()
{
    OutButt->OnClicked.AddDynamic(this,&UKMK_MakeEleWidget::ClickButt);
}

void UKMK_MakeEleWidget::ClickButt()
{
    SetVisibility(ESlateVisibility::Hidden);
    DetailHorizontalBox->ClearChildren();
    http->PotComp->ElementArray.Empty();
}

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
