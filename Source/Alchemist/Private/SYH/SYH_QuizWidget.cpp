// Fill out your copyright notice in the Description page of Project Settings.

#include "SYH/SYH_QuizWidget.h"

#include <clocale>

#include "Components/Button.h"
#include "SYH/SYH_MultiPlayer.h"
#include <windows.h>

#include "Kismet/GameplayStatics.h"


void USYH_QuizWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerController = Cast<APlayerController>(GetOwningPlayer());
	PlayerController->SetShowMouseCursor(true);
	FInputModeUIOnly input;
	PlayerController->SetInputMode(input);
	MultiPlayer = Cast<ASYH_MultiPlayer>(GetOwningPlayerPawn());
	if (Button_1)
	{
		Button_1->OnClicked.RemoveDynamic(this, &USYH_QuizWidget::OnClickedButton_1); 
		Button_1->OnClicked.AddDynamic(this, &USYH_QuizWidget::OnClickedButton_1);
	}
	if (Button_2)
	{
		Button_2->OnClicked.RemoveDynamic(this, &USYH_QuizWidget::OnClickedButton_2); 
		Button_2->OnClicked.AddDynamic(this, &USYH_QuizWidget::OnClickedButton_2);
	}
	if (Button_3)
	{
		Button_3->OnClicked.RemoveDynamic(this, &USYH_QuizWidget::OnClickedButton_3); 
		Button_3->OnClicked.AddDynamic(this, &USYH_QuizWidget::OnClickedButton_3);
	}
	TextNum = 0;
	ChangeQandA(TextNum);
}

void USYH_QuizWidget::SetQuestionText(int32 Num)
{
	TextNum = Num;
	switch(Num)
	{
	case 0:
		Question->SetText(FText::FromString(TEXT("바닷물에 많이 들어 있는 원소는 무엇일까요?")));
		break;
	case 1:
		Question->SetText(FText::FromString(TEXT("원소는 무엇일까요?")));
		break;
	case 2:
		Question->SetText(FText::FromString(TEXT("산소의 원소기호는 무엇일까요?")));
		break;
		// 다른 질문 추가 가능
	case 3:
		Question->SetText(FText::FromString(TEXT("우리 몸에서 가장 많이 있는 원소는 무엇일까요?")));
		break;
	case 4:
		Question->SetText(FText::FromString(TEXT("눈에 보이지 않지만 우리 주위에 있는 원소는 무엇일까요?")));
		break;
	}
}

void USYH_QuizWidget::SetAnswer_1Text(int32 Num)
{
	TextNum = Num;
	switch(Num)
	{
	case 0:
		Answer_1->SetText(FText::FromString(TEXT("소금(나트륨)")));
		break;
	case 1:
		Answer_1->SetText(FText::FromString(TEXT("큰 돌")));
		break;
	case 2:
		Answer_1->SetText(FText::FromString(TEXT("H")));
		break;
		// 다른 질문 추가 가능
	case 3:
		Answer_1->SetText(FText::FromString(TEXT("헬륨")));
		break;
	case 4:
		Answer_1->SetText(FText::FromString(TEXT("공기")));
		break;
	}
}

void USYH_QuizWidget::SetAnswer_2Text(int32 Num)
{
	TextNum = Num;
	switch(Num)
	{
	case 0:
		Answer_2->SetText(FText::FromString(TEXT("유리")));
		break;
	case 1:
		Answer_2->SetText(FText::FromString(TEXT("아주 작은 물질")));
		break;
	case 2:
		Answer_2->SetText(FText::FromString(TEXT("O")));
		break;
	case 3:
		Answer_2->SetText(FText::FromString(TEXT("빛")));
		break;
	case 4:
		Answer_2->SetText(FText::FromString(TEXT("책")));
		break;
	}
}

void USYH_QuizWidget::SetAnswer_3Text(int32 Num)
{
	TextNum = Num;
	switch(Num)
	{
	case 0:
		Answer_3->SetText(FText::FromString(TEXT("공기")));
		break;
	case 1:
		Answer_3->SetText(FText::FromString(TEXT("나무")));
		break;
	case 2:
		Answer_3->SetText(FText::FromString(TEXT("P")));
		break;
	case 3:
		Answer_3->SetText(FText::FromString(TEXT("물")));
		break;
	case 4:
		Answer_3->SetText(FText::FromString(TEXT("의자")));
		break;
	}
}

void USYH_QuizWidget::OnClickedButton_1()
{
	Button_1->SetIsEnabled(false);
	Button_2->SetIsEnabled(false);
	Button_3->SetIsEnabled(false);
	FTimerHandle TimerHandle;

	// 첫번쨰 문제 정답처리
	if(TextNum==0)
	{
		RightCount++;
		UE_LOG(LogTemp,Error,TEXT("quiz count : %d"),RightCount);

		// 정답 애니메이션 출력
		PlayAnimation(RightAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),RightSound);
		// 2초 딜레이 후 ChangeQandA(1) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 1), 2.0f, false);
	}
	// 두번쨰 문제 오답처리
	else if(TextNum==1)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		// 2초 딜레이 후 ChangeQandA(2) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 2), 2.0f, false);
	}
	// 세번쨰 문제 오답처리
	else if(TextNum==2)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		// 2초 딜레이 후 ChangeQandA(3) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 3), 2.0f, false);
	}
	// 네번쨰 문제 오답처리
	else if(TextNum==3)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		// 2초 딜레이 후 ChangeQandA(4) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 4), 2.0f, false);
	}
	// 마지막 문제 정답처리
	else if(TextNum ==4)
	{
		RightCount++;
		UE_LOG(LogTemp,Error,TEXT("quiz count : %d"),RightCount);
		// 정답 애니메이션 출력
		PlayAnimation(RightAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),RightSound);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,&USYH_QuizWidget::LastQ, 2.0f,false);
	}
}

void USYH_QuizWidget::OnClickedButton_2()
{
	Button_1->SetIsEnabled(false);
	Button_2->SetIsEnabled(false);
	Button_3->SetIsEnabled(false);
	FTimerHandle TimerHandle;

	// 첫번쨰 문제 오답처리
	if(TextNum==0)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		// 2초 딜레이 후 ChangeQandA(1) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 1), 2.0f, false);
	}
	// 두번쨰 문제 정답처리
	else if(TextNum==1)
	{
		RightCount++;
		UE_LOG(LogTemp,Error,TEXT("quiz count : %d"),RightCount);
		
		// 정답 애니메이션 출력
		PlayAnimation(RightAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),RightSound);

		// 2초 딜레이 후 ChangeQandA(2) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 2), 2.0f, false);
	}
	// 세번쨰 문제 정답처리
	else if(TextNum==2)
	{
		RightCount++;
		UE_LOG(LogTemp,Error,TEXT("quiz count : %d"),RightCount);

		// 정답 애니메이션 출력
		PlayAnimation(RightAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),RightSound);
		// 2초 딜레이 후 ChangeQandA(3) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 3), 2.0f, false);
	}
	// 네번쨰 문제 오답처리
	else if(TextNum==3)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		// 2초 딜레이 후 ChangeQandA(4) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 4), 2.0f, false);
	}
	// 마지막 문제 오답처리
	else if(TextNum ==4)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,&USYH_QuizWidget::LastQ, 2.0f,false);
	}
}

void USYH_QuizWidget::OnClickedButton_3()
{
	Button_1->SetIsEnabled(false);
	Button_2->SetIsEnabled(false);
	Button_3->SetIsEnabled(false);
	FTimerHandle TimerHandle;
	if(TextNum==0)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);

		// 2초 딜레이 후 ChangeQandA(1) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 1), 2.0f, false);
	}
	// 두번쨰 문제 오답처리
	else if(TextNum==1)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);

		// 2초 딜레이 후 ChangeQandA(2) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 2), 2.0f, false);
	}
	// 세번쨰 문제 오답처리
	else if(TextNum==2)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);

		// 2초 딜레이 후 ChangeQandA(3) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 3), 2.0f, false);
	}
	// 네번쨰 문제 정답처리
	else if(TextNum==3)
	{
		RightCount++;
		UE_LOG(LogTemp,Error,TEXT("quiz count : %d"),RightCount);

		// 정답 애니메이션 출력
		PlayAnimation(RightAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),RightSound);

		// 2초 딜레이 후 ChangeQandA(4) 호출
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &USYH_QuizWidget::ChangeQandA, 4), 2.0f, false);
	}
	// 마지막 문제 오답처리
	else if(TextNum ==4)
	{
		// 오답 애니메이션 출력
		PlayAnimation(WrongAnim);
		UGameplayStatics::PlaySound2D(GetWorld(),WrongSound);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this,&USYH_QuizWidget::LastQ, 2.0f,false);

	}
}

void USYH_QuizWidget::ChangeQandA(int32 Num)
{
	Button_1->SetIsEnabled(true);
	Button_2->SetIsEnabled(true);
	Button_3->SetIsEnabled(true);
	SetQuestionText(Num);
	SetAnswer_1Text(Num);
	SetAnswer_2Text(Num);
	SetAnswer_3Text(Num);
}

void USYH_QuizWidget::LastQ()
{
	RemoveFromParent();
	if(MultiPlayer) // 요청을 받은 사람
	{
		MultiPlayer->ServerRPC_SendRightCount(RightCount);
		RightCount = 0;
	}
}
