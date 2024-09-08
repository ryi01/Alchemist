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
    DeletButt->OnClicked.AddDynamic(this,&UKMK_StudyWidget::ClickDel);
}


void UKMK_StudyWidget::ClickDel()
{
    RemoveFromParent();
}


void UKMK_StudyWidget::SetMyText(const FString& name, const FString& text, class UTexture2D *myTexture)
{
    ExplainText->SetIsReadOnly(true);
    ElementName->SetText(FText::FromString(name));
    ExplainText->SetText(FText::FromString(text));
    Image3D->SetBrushFromTexture(myTexture);
}

