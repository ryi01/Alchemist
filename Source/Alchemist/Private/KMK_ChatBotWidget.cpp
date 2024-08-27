// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_ChatBotWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "KMK_TextWidget.h"

void UKMK_ChatBotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // 버튼 바인딩
    SendButt->OnClicked.AddDynamic(this, &UKMK_ChatBotWidget::OnClickSendButt);
}

void UKMK_ChatBotWidget::OnClickSendButt()
{
    // 텍스트가 비어있지 않은 경우
    if (!PlayerChat->GetText().IsEmpty())
    {
        // 텍스트 위잿 생성
        auto* widget = Cast<UKMK_TextWidget>(CreateWidget(GetWorld(), ChatTextWidFact));
        if (widget)
        {
            // 위잿 텍스트 작성
            widget->SetChatText(PlayerChat->GetText());
            widget->AddToViewport();
            // 스크롤 밑에 생성되게 만들기
            ChatLog->AddChild(widget);
            PlayerChat->SetText(FText::GetEmpty());
        }
    }
}
