// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "KMK_TextWidget.h"
#include "Components/WidgetComponent.h"
#include "KMK_SingleIntaraction.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UKMK_SingleIntaraction : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_SingleIntaraction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 엑터에 붙어있는 친구
	// 각각 widget을 설정해줘야함 => 플레이어 클릭에 따라 달라지기 때문
	UPROPERTY(EditAnywhere, Category = "WidgetBluePrint")
	TSubclassOf<class UUserWidget>widgetFact;
	
	UPROPERTY(EditAnywhere, Category = "WidgetBluePrint")
	class UKMK_TextWidget* textWidget;

	UPROPERTY(EditAnywhere)
	bool bClickActor = false;

	UPROPERTY(EditAnywhere)
	bool bMouseOnActor = false;
		
	UFUNCTION()
	void CreatePlayerWidget(bool bMake, int num,APlayerController* pc);
	
	UFUNCTION()
	void OnCreateNameWidget(bool bActive)
	{
		bMouseOnActor = bActive;
		if (bActive)
		textWidget->SetChatText(*GetOwner()->GetActorLabel());
		else textWidget->SetChatText(TEXT(""));
	}
};
