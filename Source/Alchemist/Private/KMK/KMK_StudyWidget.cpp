// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_StudyWidget.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"

void UKMK_StudyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 바인딩
    NextButt->OnClicked.AddDynamic(this,&UKMK_StudyWidget::ClickNext);
    PreButt->OnClicked.AddDynamic(this,&UKMK_StudyWidget::ClickPre);
    DeletButt->OnClicked.AddDynamic(this,&UKMK_StudyWidget::ClickDel);
}

void UKMK_StudyWidget::ClickNext()
{

}

void UKMK_StudyWidget::ClickPre()
{

}

void UKMK_StudyWidget::ClickDel()
{
    RemoveFromParent();
    if(me != nullptr) me->SetPause(false);
}

void UKMK_StudyWidget::SetButtVisi(bool isActive,APlayerController* pc)
{
    ExplainText->SetIsEnabled(false);
    if(pc != nullptr) me = pc;
    if ( isActive )
    {
        NextButt->SetVisibility(ESlateVisibility::Hidden);
        PreButt->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        NextButt->SetVisibility(ESlateVisibility::Visible);
        PreButt->SetVisibility(ESlateVisibility::Visible);
        DeletButt->SetVisibility(ESlateVisibility::Visible);
    }
}
