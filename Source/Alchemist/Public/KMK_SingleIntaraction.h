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
	void OnCreateMyWidget(bool bMake, int num = 0)
	{
		auto* wid = CreateWidget(GetWorld(), widgetFact);
		// 책상 클릭시 카메라 변경
		if (bMake)
		{
			// 데스크 카메라 활성화
			wid->AddToViewport(num);
		}
		else
		{
			// 플레이어 카메라 활성화
			wid->RemoveFromParent();
		}
	}
	
	
	UFUNCTION()
	void OnCreateNameWidget(bool bActive)
	{
		if (bActive)
		textWidget->SetChatText(FText::FromString(*GetOwner()->GetActorLabel()));
		else textWidget->SetChatText(FText::GetEmpty());
	}
};
