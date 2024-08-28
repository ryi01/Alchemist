// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Guide_MainWidget.generated.h"

/** 
 * 메인 위젯은 화면에 도감 UI를 띄우는 위젯
 */
UCLASS()
class ALCHEMIST_API UGuide_MainWidget : public UUserWidget
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
};
