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
	class UImage* Image3D;
	UPROPERTY(meta = (BindWidget ))
	class UMultiLineEditableTextBox* ExplainText;

	FTextBlockStyle fontInfo;

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
	void SetButtVisi(bool isActive, APlayerController* pc, class UKMK_PlayerMouse* pm,int num = -1);
	// 원소 위치 및 newElement저장

	UPROPERTY(EditAnywhere)
	UMaterialInstance* instnaceMat;
	UPROPERTY(EditAnywhere)
	TArray<UTexture2D*> imageMats;
	UPROPERTY(EditAnywhere)
	TArray<FString> studyText;

	int index = 0;
	UPROPERTY()
	class UKMK_PlayerMouse* PlayerMouseComp;

	UFUNCTION()
	void SetMyText(const FString& name, const FString& text,class UTexture2D *myTexture);

	void ChangePage(int pageNo);

	void AddStringText();

};
