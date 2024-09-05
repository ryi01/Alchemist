// Fill out your copyright notice in the Description page of Project Settings.

#include "SYH/SYH_QuizWidget.h"

#include <clocale>

#include "Components/Button.h"
#include "SYH/SYH_MultiPlayer.h"
#include <windows.h>


void USYH_QuizWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	PlayerController->SetShowMouseCursor(true);
	FInputModeUIOnly input;
	PlayerController->SetInputMode(input);

	MultiPlayer = Cast<ASYH_MultiPlayer>(PlayerController->GetLocalPlayer());
	if (Button_1)
	{
		Button_1->OnClicked.AddDynamic(this, &USYH_QuizWidget::OnClickedButton_1);
	}
	if (Button_2)
	{
		Button_2->OnClicked.AddDynamic(this, &USYH_QuizWidget::OnClickedButton_2);
	}
	if (Button_3)
	{
		Button_3->OnClicked.AddDynamic(this, &USYH_QuizWidget::OnClickedButton_3);
	}
	
	if(Question && Button_1 && Button_2 && Button_3)
	{
		QFontInfo = Question->GetFont();
		AFontInfo = Answer_1->GetFont();
	}
}

void USYH_QuizWidget::SetQuestionText(int32 TextNum)
{
	switch(TextNum)
	{
	case 1:
		Question->SetFont(QFontInfo);
		Question->SetText(FText::FromString("원소는 무엇일까요?"));
		break;
	case 2:
		Question->SetText(FText::FromString("다음 중 공기 중에 있는 원소는 무엇일까요?"));
		break;
		// 다른 질문 추가 가능
	case 3:
		Question->SetText(FText::FromString("우리 몸에서 가장 많이 있는 원소는 무엇일까요?"));
		break;
	case 4:
		Question->SetText(FText::FromString("눈에 보이지 않지만 우리 주위에 있는 원소는 무엇일까요?"));
		break;
	}
}

void USYH_QuizWidget::SetAnswer_1Text(int32 TextNum)
{
	switch(TextNum)
	{
	case 1:
		Answer_1->SetFont(AFontInfo);
		Answer_1->SetText(FText::FromString("아주 작은 물질")); //
		break;
	case 2:
		Answer_1->SetFont(AFontInfo);
		Answer_1->SetText(FText::FromString("철"));
		break;
		// 다른 질문 추가 가능
	case 3:
		Answer_1->SetFont(AFontInfo);
		Answer_1->SetText(FText::FromString("물"));
		break;
	case 4:
		Answer_1->SetFont(AFontInfo);
		Answer_1->SetText(FText::FromString("공기"));
		break;
	}
}

void USYH_QuizWidget::SetAnswer_2Text(int32 TextNum)
{
	switch(TextNum)
	{
	case 1:
		Answer_2->SetFont(AFontInfo);
		Answer_2->SetText(FText::FromString("큰 돌")); //
		break;
	case 2:
		Answer_2->SetFont(AFontInfo);
		Answer_2->SetText(FText::FromString("산소"));
		break;
		// 다른 질문 추가 가능
	case 3:
		Answer_2->SetFont(AFontInfo);
		Answer_2->SetText(FText::FromString("산소"));
		break;
	case 4:
		Answer_2->SetFont(AFontInfo);
		Answer_2->SetText(FText::FromString("책"));
		break;
	}
}

void USYH_QuizWidget::SetAnswer_3Text(int32 TextNum)
{
	switch(TextNum)
	{
	case 1:
		Answer_3->SetFont(AFontInfo);
		Answer_3->SetText(FText::FromString("나무")); //
		break;
	case 2:
		Answer_3->SetFont(AFontInfo);
		Answer_3->SetText(FText::FromString("금"));
		break;
		// 다른 질문 추가 가능
	case 3:
		Answer_3->SetFont(AFontInfo);
		Answer_3->SetText(FText::FromString("나무"));
		break;
	case 4:
		Answer_3->SetFont(AFontInfo);
		Answer_3->SetText(FText::FromString("의자"));
		break;
	}
}

void USYH_QuizWidget::OnClickedButton_1()
{
	// 첫번쨰 문제 정답처리
	SetQuestionText(1);
	SetAnswer_1Text(1);
	SetAnswer_2Text(1);
	SetAnswer_3Text(1);
}

void USYH_QuizWidget::OnClickedButton_2()
{
	
}

void USYH_QuizWidget::OnClickedButton_3()
{
	
}
