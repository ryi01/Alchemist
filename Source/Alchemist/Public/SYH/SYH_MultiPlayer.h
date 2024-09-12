
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* IA_Menu;

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

	void MyJump(const FInputActionValue& Value);

	void OnMyCheckActor();

	void Quiz(const FInputActionValue& Value);

	void Menu(const FInputActionValue& Value);

	// void Quiz(const FInputActionValue& Value);
	UPROPERTY()
	class AAluminum_Object* Aluminum;

	int count = 0;

public:
	UPROPERTY(EditAnywhere)
	bool InQuiz;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quiz")
	class ASYH_MultiPlayer* me;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quiz")
	class ASYH_MultiPlayer* TargetPlayer;
	// 퀴즈 관련 함수
	// 특정거리내에 들어오면 F키를 누르라는 안내 UI를 띄우고싶다.
	UFUNCTION(NetMulticast,Reliable)
	void ClientRPC_CallFKey();
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> QuizWaitClass;
	UPROPERTY()
	class USYH_QuizWaitWidget* QuizWaitWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> QuizSelectClass;
	UPROPERTY()
	class USYH_QuizSelect* QuizSelectWidget;
	class UGuide_GameInstance* GameInstance;
	UPROPERTY(EditDefaultsOnly)
	float RequestUIDistance = 200.0f;
	void CheckDist(bool bCheck);

	UFUNCTION(Client,Reliable)
	void ClientRPC_ShowQuizSelect();

	UFUNCTION(Client,Reliable)
	void ClientRPC_ShowQuizWait();

	FTimerHandle Timer;
	void HideQuizReject();
	UFUNCTION(Server,Reliable,withValidation)
	void ServerRPC_Quiz();

	void Server_Quiz();

	// select 버튼 이벤트

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> QuizClass;
	UPROPERTY()
	class USYH_QuizWidget* QuizWidget;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_AcceptQuiz();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_RejectQuiz();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowQuiz();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowQuizReject();

	// 승자 패자 구별
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> QuizResultClass;
	UPROPERTY()
	class USYH_QuizWidgetResult* QuizResultWidget;
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRPC_SendRightCount(int32 Count);

	int32 RightCount = -1;

	void Server_Compare();

	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowWinResult();
	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowLoseResult();
	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowSameResult();
	UFUNCTION(Client, Reliable)
	void ClientRPC_ShowWaitResult();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Replicated)
	bool IsWin;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Replicated)
	bool IsLose;
	//esc
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MenuClass;
	UPROPERTY()
	class USYH_MenuWidget* MenuWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlayerInteractionComponent* interactionComp;
};
