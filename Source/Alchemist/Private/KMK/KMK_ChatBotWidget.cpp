// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_ChatBotWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "KMK/KMK_TextWidget.h"
#include "KMK/KMK_HttpActorWithAI.h"
#include "Components/ScrollBoxSlot.h"

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
        // AI에게 정보값 보내기
        MakeChatText(PlayerChat->GetText());
        PlayerChat->SetIsEnabled(false);
        httpActor->ReqChatBot(*PlayerChat->GetText().ToString());
        PlayerChat->SetText(FText::GetEmpty());
    }
}

void UKMK_ChatBotWidget::MakeChatText(FText text, int num)
{
    // 텍스트 위잿 생성
    auto* widget = Cast<UKMK_TextWidget>(CreateWidget(GetWorld(), ChatTextWidFact));
    if ( widget )
    {
        // 위잿 텍스트 작성
        widget->SetChatText(text, count % 2);
        widget->AddToViewport();

        ChatLog->AddChild(widget);
        UScrollBoxSlot* a = CastChecked<UScrollBoxSlot>(widget->Slot);
        // Child의 슬롯을 가져옴
        if ( a )
        {
            // 슬롯의 Alignment 설정
            if ( count % 2 == 0 )a->SetHorizontalAlignment(HAlign_Right);
            else a->SetHorizontalAlignment(HAlign_Left);
            // 스크롤 밑에 생성되게 만들기
            count++;
        }
    }
    
}

void UKMK_ChatBotWidget::SetHttpActor(AKMK_HttpActorWithAI* actor)
{
    httpActor = actor;
    httpActor->HttpUI = this;
}
