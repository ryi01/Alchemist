// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_ChatBotWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableText.h"
#include "KMK_TextWidget.h"

void UKMK_ChatBotWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // ��ư ���ε�
    SendButt->OnClicked.AddDynamic(this, &UKMK_ChatBotWidget::OnClickSendButt);
}

void UKMK_ChatBotWidget::OnClickSendButt()
{
    // �ؽ�Ʈ�� ������� ���� ���
    if (!PlayerChat->GetText().IsEmpty())
    {
        // �ؽ�Ʈ ���� ����
        auto* widget = Cast<UKMK_TextWidget>(CreateWidget(GetWorld(), ChatTextWidFact));
        if (widget)
        {
            // ���� �ؽ�Ʈ �ۼ�
            widget->SetChatText(PlayerChat->GetText());
            widget->AddToViewport();
            // ��ũ�� �ؿ� �����ǰ� �����
            ChatLog->AddChild(widget);
            PlayerChat->SetText(FText::GetEmpty());
        }
    }
}
