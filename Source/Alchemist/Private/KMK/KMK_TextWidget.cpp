// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_TextWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/MultiLineEditableTextBox.h"

// 텍스트 설정 함수
void UKMK_TextWidget::SetChatText(FString text)
{
    ResChatText->SetText(FText::FromString(text));
}
