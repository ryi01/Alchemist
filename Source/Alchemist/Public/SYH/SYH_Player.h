// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SYH_Player.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplate, Log, All);

UCLASS(config=Game)
class ASYH_Player : public ACharacter
{
	GENERATED_BODY()

public:


	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* IMC_Player;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Jump;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Move;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Mouse;

public:
	ASYH_Player();
	virtual void PossessedBy(AController* NewController) override;
	UPROPERTY()
	class USYH_PlayerAnim* anim;
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	class UKMK_SingleIntaraction* interActor;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	

	void OnClickedLeft(const FInputActionValue& Value);

	void OnMyCheckActor();

	void CreatePopUpWidget();
	
	int count = 0;
	bool bCreateWidget = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UUserWidget> PopUPWidgetClass;
	UPROPERTY();
	class UKMK_PopUpWidget* PopUpWidget;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;


};

