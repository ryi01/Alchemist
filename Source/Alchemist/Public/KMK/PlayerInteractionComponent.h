﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h"
#include "PlayerInteractionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY()
	class ASYH_MultiPlayer* me;
	void SetupInputBinding(class UEnhancedInputComponent* input);
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* IA_Interaction;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> widget;

	void OnMyActionInteraction(const FInputActionValue& Value);
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	class UKMK_SingleIntaraction* interActor;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* DeskActor;
	int count = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UUserWidget> PopUPWidgetClass;
	UPROPERTY();
	class UKMK_PopUpWidget* PopUpWidget;
	UPROPERTY()
   class UKMK_PlayerMouse* deskMouse;

   void OnMyCheckActor();

	UPROPERTY(BlueprintReadWrite)
	TArray<FString> collectionTag;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UKMK_GrabActorComp* potComp;
	UPROPERTY()
	int textNum = 0;

	UPROPERTY(BlueprintReadWrite)
	class UMissionWidget* missionWidget;

	UFUNCTION()
	void CreateMainWidget();

	UFUNCTION()
	void DeleteMainWidget();

	UPROPERTY()
	class UKMK_DeskComponent* desk;

	bool bMakeMainWidget = false;

	// ===========================================================
	UFUNCTION(Server, Reliable)
	void ServerRPCSectionOpen(class ASectionActor* target);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPCSecionOpen(class ASectionActor* target);

	void RemoveCollision(class ASectionActor* target);
};
