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
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;
	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraCompThird;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* CameraCompFirst;
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
	UInputAction* IA_Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Mouse;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Guide;
public:
	ASYH_Player();
	virtual void PossessedBy(AController* NewController) override;
	UPROPERTY()
	class USYH_PlayerAnim* anim;
	UPROPERTY()
	class APlayerController* player;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget* CameraWidget;
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	class UKMK_SingleIntaraction* interActor;
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	void Camera(const FInputActionValue& Value);

	void OnClickedLeft(const FInputActionValue& Value);

	void OnMyCheckActor();

	void CreatePopUpWidget();
	
	int count = 0;
	bool bCreateWidget = false;

	// 도감
	void OnOffGuide(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGuide_MainWidget> GuideWidgetClass;
	UPROPERTY()
	class UGuide_MainWidget* GuideWidget;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArmComp() const { return SpringArmComp; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetCameraCompThird() const { return CameraCompThird; }
};

