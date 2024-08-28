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
	
	// ���Ϳ� �پ��ִ� ģ��
	// ���� widget�� ����������� => �÷��̾� Ŭ���� ���� �޶����� ����
	UPROPERTY(EditAnywhere, Category = "WidgetBluePrint")
	class UUserWidget* widgetFact;
	UPROPERTY(EditAnywhere, Category = "WidgetBluePrint")
	class UKMK_TextWidget* textWidget;

	UPROPERTY(EditAnywhere)
	bool bClickActor = false;

	UPROPERTY(EditAnywhere)
	bool bMouseOnActor = false;

	UFUNCTION()
	void OnCreateMyWidget(bool bMake, UUserWidget* wid, int num = 0)
	{
		// å�� Ŭ���� ī�޶� ����
		if (bMake)
		{
			// ����ũ ī�޶� Ȱ��ȭ
			wid->AddToViewport(num);
		}
		else
		{
			// �÷��̾� ī�޶� Ȱ��ȭ
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
