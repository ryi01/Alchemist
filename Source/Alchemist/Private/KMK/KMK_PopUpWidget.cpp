// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_PopUpWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SYH/SYH_Player.h"

void UKMK_PopUpWidget::NativeConstruct()
{
    Super::NativeConstruct();
    PlayerController = Cast<APlayerController>(GetOwningPlayer());
    
    if (YesButt)
    {
        YesButt->OnClicked.AddDynamic(this, &UKMK_PopUpWidget::OnClickedYesButt);
    }
  
    if (NoButt)
    {
        NoButt->OnClicked.AddDynamic(this, &UKMK_PopUpWidget::OnClickedNobutt);
    }
    if (PauseButt)
    {
        PauseButt->OnClicked.AddDynamic(this, &UKMK_PopUpWidget::OnClickedPausebutt);
    }
    if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer)
    {
        // 서버 코드
    }
    else
    {
        // 클라이언트 코드
        VisibleYesButt();
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

void UKMK_PopUpWidget::OnClickedPausebutt()
{
    
    if (GetWorld()->GetNetMode() == NM_DedicatedServer || GetWorld()->GetNetMode() == NM_ListenServer)
    {
        // 서버 코드
        RemoveFromParent();
    }
    else
    {
        // 클라이언트 코드
        RemoveFromParent();
    }
}

void UKMK_PopUpWidget::VisibleYesButt()
{
    if(YesButt)
    {
        YesButt->SetVisibility(ESlateVisibility::Hidden);
    }
}

