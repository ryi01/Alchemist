// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KMK_ChatBotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ALCHEMIST_API UKMK_ChatBotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SendButt;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* ChatLog;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableText* PlayerChat;
	UPROPERTY(EditAnywhere, Category = "WBP_Chat")
	TSubclassOf<class UUserWidget> ChatTextWidFact;

	UFUNCTION()
	void OnClickSendButt();

};
