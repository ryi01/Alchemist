// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK_StudyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_StudyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	UPROPERTY(meta = (BindWidget ))
	class UButton* NextButt;
	UPROPERTY(meta = (BindWidget ))
	class UButton* PreButt;
	UPROPERTY(meta = (BindWidget ))
	class UButton* DeletButt;
	UPROPERTY(meta = (BindWidget ))
	class UTextBlock* ElementName;
	UPROPERTY(meta = (BindWidget ))
	class UMultiLineEditableTextBox* ExplainText;

	UFUNCTION()
	void ClickNext();
	UFUNCTION()
	void ClickPre();
	UFUNCTION()
	void ClickDel();
	UPROPERTY()
	class UKMK_ElementGameActor* newElement;
	UPROPERTY()
	APlayerController* me;
	UFUNCTION()
	void SetButtVisi(bool isActive, APlayerController* pc);

	// 원소 이름 및 설명 담을 변수들

};
