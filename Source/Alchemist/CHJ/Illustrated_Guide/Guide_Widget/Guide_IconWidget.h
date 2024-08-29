// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Guide_IconWidget.generated.h"

class UImage;
class USizeBox;
/**
 * 아이콘 위젯은 아이템 위젯
 */
UCLASS()
class ALCHEMIST_API UGuide_IconWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;

	

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Slot;
};
