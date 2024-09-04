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
        MakeChatText(PlayerChat->GetText().ToString());
        PlayerChat->SetIsEnabled(false);
        httpActor->ReqChatBot1(*PlayerChat->GetText().ToString());
        PlayerChat->SetText(FText::GetEmpty());
    }
}

void UKMK_ChatBotWidget::MakeChatText(FString text, int num)
{

    auto* widget = Cast<UKMK_TextWidget>(CreateWidget(GetWorld(), ChatTextWidFact[num]));
    if ( widget )
    {
        widget->AddToViewport();
        ChatLog->AddChild(widget);
        if(widget->Slot != nullptr) a = CastChecked<UScrollBoxSlot>(widget->Slot);
        // Child의 슬롯을 가져옴
        if ( num == 0 )
        {
            // 위잿 텍스트 작성
            widget->SetChatText(text);
            // 슬롯의 Alignment 설정
            a->SetHorizontalAlignment(HAlign_Right);
           
            // 스크롤 밑에 생성되게 만들기
        }
        else
        {
            // 위잿 텍스트 작성
            widget->SetChatText(text);
            a->SetHorizontalAlignment(HAlign_Left);
        }
        
        count++;

    }
    
}

void UKMK_ChatBotWidget::SetHttpActor(AKMK_HttpActorWithAI* actor)
{
    httpActor = actor;
    httpActor->HttpUI = this;
}
