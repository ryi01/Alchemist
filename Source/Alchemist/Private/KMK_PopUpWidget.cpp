// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PopUpWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UKMK_PopUpWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 바인딩
    if (YesButt)
    {
        // 버튼 바인딩
        YesButt->OnClicked.AddDynamic(this, &UKMK_PopUpWidget::OnClickedYesButt);
    }
    if (NoButt)
    {
        // 버튼 바인딩
        NoButt->OnClicked.AddDynamic(this, &UKMK_PopUpWidget::OnClickedNobutt);
    }
}

void UKMK_PopUpWidget::SetPopUpText(FText text)
{
    PopUpText->SetText(text);
}

void UKMK_PopUpWidget::OnClickedYesButt()
{

}

void UKMK_PopUpWidget::OnClickedNobutt()
{

}
