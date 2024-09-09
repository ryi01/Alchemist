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
	class UButton* DeletButt;
	UPROPERTY(meta = (BindWidget ))
	class UTextBlock* ElementName;
	UPROPERTY(meta = (BindWidget ))
	class UImage* Image3D;
	UPROPERTY(meta = (BindWidget ))
	class UMultiLineEditableTextBox* ExplainText;

	UFUNCTION()
	void ClickDel();
	// 원소 위치 및 newElement저장

	UFUNCTION()
	void SetMyText(const FString& name, const FString& text,class UTexture2D *myTexture);

};
