// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_TextWidget.h"
#include "Components/TextBlock.h"

// 텍스트 설정 함수
void UKMK_TextWidget::SetChatText(FText text)
{
    ChatText->SetText(text);
}
