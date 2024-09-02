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

	UFUNCTION()
	void ClickNext();
	UFUNCTION()
	void ClickPre();

	// 원소 이름 및 설명 담을 변수들

};
