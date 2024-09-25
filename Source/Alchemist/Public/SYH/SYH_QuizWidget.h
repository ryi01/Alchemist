// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "SYH_QuizWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API USYH_QuizWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FTimerHandle Timer;
	virtual void NativeConstruct() override;
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY()
	class ASYH_MultiPlayer* MultiPlayer;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Question;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_1;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_2;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_3;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Answer_1;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Answer_2;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Answer_3;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* WrongAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* RightAnim;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USoundBase* RightSound;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USoundBase* WrongSound;
	int32 TextNum;
	UFUNCTION()
	void SetQuestionText(int32 Num);
	UFUNCTION()
	void SetAnswer_1Text(int32 Num);
	UFUNCTION()
	void SetAnswer_2Text(int32 Num);
	UFUNCTION()
	void SetAnswer_3Text(int32 Num);
	UFUNCTION()
	void OnClickedButton_1();
	UFUNCTION()
	void OnClickedButton_2();
	UFUNCTION()
	void OnClickedButton_3();

	void ChangeQandA(int32 Num);

	int32 RightCount = 0;

	UFUNCTION()
	void LastQ();
	
};
