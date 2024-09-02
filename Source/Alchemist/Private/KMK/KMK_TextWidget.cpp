// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_TextWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"

// 텍스트 설정 함수
void UKMK_TextWidget::SetChatText(FText text , int num)
{
    if ( mats.Num() > 0 )
    {
         BackTextImage->SetBrushFromTexture(mats[num]);
         // VerticalBox에서 지정된 인덱스의 Child를 가져옴
         count++;
         UWidget* ChildWidget = TextVerticalBox->GetChildAt(count);

         if ( ChildWidget )
         {
             // Child의 슬롯을 가져옴
             UVerticalBoxSlot* childSlot = Cast<UVerticalBoxSlot>(ChildWidget->Slot);

             if ( childSlot )
             {
                 // 슬롯의 Alignment 설정
                 if(num %2 == 0)childSlot->SetHorizontalAlignment(HAlign_Right);
                 else childSlot->SetHorizontalAlignment(HAlign_Left);
             }
         }
    }
    ChatText->SetText(text);

}
