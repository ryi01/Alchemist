
#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SYH_MultiPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ASYH_MultiPlayer : public ACharacter
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
	class UCameraComponent* CameraCompFirst;
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
	UInputAction* IA_Guide;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Quiz;

	// Sets default values for this character's properties
	ASYH_MultiPlayer();
	virtual void PossessedBy(AController* NewController) override;
	UPROPERTY()
	class USYH_PlayerAnim* anim;
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UUserWidget* CameraWidget;
	UPROPERTY()
	FHitResult HitResult;
	UPROPERTY()
	class UKMK_SingleIntaraction* interActor;

	// 도감
	void OnOffGuide(const FInputActionValue& Value);
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UGuide_MainWidget> GuideWidgetClass;
	UPROPERTY()
	class UGuide_MainWidget* GuideWidget;


protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	void Camera(const FInputActionValue& Value);

	void OnMyCheckActor();

	void CreatePopUpWidget();
	

	void Quiz(const FInputActionValue& Value);
	UPROPERTY()
	class AAluminum_Object* Aluminum;
	
	int count = 0;

public:
	//UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	//bool bIsCamera = false;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetSpringArmComp() const { return SpringArmComp; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetCameraCompThird() const { return CameraCompThird; }

	void ObjectDetect();

	// 퀴즈 관련 함수

	// 특정거리내에 들어오면 F키를 누르라는 안내 UI를 띄우고싶다.
	UFUNCTION(Client,Reliable)
	void CallQuizRequestUI(); 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> QuizWaitClass;
	UPROPERTY()
	class USYH_QuizWaitWidget* QuizWaitWidget;

	UPROPERTY(EditDefaultsOnly)
	float RequestUIDistance = 500.0f;

	void CheckRequestDistance();

	// F키를 누르면 서버에 퀴즈 요청을 보내고 싶다.
	UFUNCTION(Server,reliable,withValidation)
	void SendQuizRequest(FVector start, FVector end, float radius);
	// 퀴즈 요청을 다루는 함수
	void HandleQuizRequest(AActor* TargetActor);
	
	class UGuide_GameInstance* GameInstance;

	UFUNCTION(Client,Reliable)	
	void ShowQuizRequestUI();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> QuizRequestClass;
	UPROPERTY()
	class USYH_QuizSelect* QuizRequestWidget;
	
};
