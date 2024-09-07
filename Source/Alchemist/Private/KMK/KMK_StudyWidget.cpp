// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_StudyWidget.h"
#include "Components/Button.h"
#include "Components/MultiLineEditableTextBox.h"
#include "KMK/KMK_ElementGameActor.h"
#include "KMK/KMK_PlayerMouse.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

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
    if ( PlayerMouseComp != nullptr )
    {
        PlayerMouseComp->cnt = 0;
        PlayerMouseComp->IsRay = false;
    }
}

void UKMK_StudyWidget::SetButtVisi(bool isActive,APlayerController* pc, class UKMK_PlayerMouse* pm, int num)
{
    ExplainText->SetIsReadOnly(true);
    if(pc != nullptr) me = pc;
    if ( pm != nullptr )
    {
        PlayerMouseComp = pm;
    }
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

void UKMK_StudyWidget::SetMyText(const FString& name, const FString& text, class UTexture2D *myTexture)
{
    ElementName->SetText(FText::FromString(name));
    ExplainText->SetText(FText::FromString(text));
    Image3D->SetBrushFromTexture(myTexture);
}

