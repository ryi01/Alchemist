// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_TextWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/MultiLineEditableTextBox.h"

// 텍스트 설정 함수
void UKMK_TextWidget::SetChatText(FString text , int num)
{
    if ( mats.Num() > 0 )
    {
         BackTextImage->SetBrushFromTexture(mats[0]);
         // VerticalBox에서 지정된 인덱스의 Child를 가져옴
         count++;
         UWidget* ChildWidget = TextVerticalBox->GetChildAt(count);

         if ( ChildWidget )
         {
             // Child의 슬롯을 가져옴
             UVerticalBoxSlot* childSlot = Cast<UVerticalBoxSlot>(ChildWidget->Slot);

             if ( childSlot )
             {
                
                 if ( num == 0 )
                 {
                     SetVisibleText(childSlot, HAlign_Right, ESlateVisibility::Visible);
                     ChatBotTextureBox->SetVisibility(ESlateVisibility::Hidden);
                     ChatText->SetText(FText::FromString(text));
                 }
                 else
                 {
                     SetVisibleText(childSlot,HAlign_Left,ESlateVisibility::Hidden); 
                     ChatBotTextureBox->SetVisibility(ESlateVisibility::Visible);
                     ResChatText->SetText(FText::FromString(text));
                 }
             }
         }
    }
    else
    {
        ChatText->SetText(FText::FromString(text));
    }

}

void UKMK_TextWidget::SetVisibleText(UVerticalBoxSlot* childSlot, EHorizontalAlignment align, ESlateVisibility visible)
{
    childSlot->SetHorizontalAlignment(align);
    BackTextImage->SetVisibility(visible);
    ChatText->SetVisibility(visible);
}

