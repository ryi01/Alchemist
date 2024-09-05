// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_TextWidget.h"
#include "KMK_ElementGameActor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UKMK_ElementGameActor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_ElementGameActor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// 생성된 엑터에 정보값을 저장할 예정 => Map 형태
	TMap<FString,TMap<FString,FString>> myInfos;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* widComp;

	UPROPERTY(EditAnywhere, Category = "WidgetBluePrint")
	class UKMK_TextWidget* textWidget;

	UPROPERTY(BlueprintReadWrite)
	bool isUp = false;
	// 위치 수정할 수 있는 함수
	UFUNCTION()
	void ChangeMyPos(FVector myPos);

	UPROPERTY()
	bool isOnWidget = false;

	UFUNCTION()
	void OnCreateWidget(bool isActive, FString text)
	{
		isOnWidget = isActive;
		if ( isActive )
		{
			textWidget->SetVisibility(ESlateVisibility::Visible);
			textWidget->SetChatText(text);

		}
		else
		{
			textWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
		
};
