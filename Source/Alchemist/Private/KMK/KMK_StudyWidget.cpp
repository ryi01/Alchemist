// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_StudyWidget.h"
#include "Components/Button.h"

void UKMK_StudyWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 바인딩
    NextButt->OnClicked.AddDynamic(this,&UKMK_StudyWidget::ClickNext);
    PreButt->OnClicked.AddDynamic(this,&UKMK_StudyWidget::ClickPre);
}

void UKMK_StudyWidget::ClickNext()
{

}

void UKMK_StudyWidget::ClickPre()
{

}
