#include "SYH/SYH_MultiPlayer.h"

#include "EngineUtils.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "SYH/CameraWidget.h"
#include "SYH/SYH_PlayerAnim.h"
#include "KMK/KMK_SingleIntaraction.h"
#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Aluminum_Object.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "SYH/SYH_QuizSelect.h"
#include "SYH/SYH_QuizWaitWidget.h"
// Sets default values
DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ASYH_MultiPlayer::ASYH_MultiPlayer()
{
 	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = false; 
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 150.0f; // The camera follows at this distance behind the character	
	SpringArmComp->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	SpringArmComp->SetRelativeLocation(FVector(0,40,60));

	// Create a follow camera
	CameraCompThird = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCompThird"));
	CameraCompThird->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraCompThird->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	CameraCompFirst = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraCompFirst"));
	CameraCompFirst->SetupAttachment(GetMesh()); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	CameraCompFirst->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	CameraCompFirst->SetActive(false);
	CameraCompFirst->SetRelativeLocationAndRotation(FVector(0, 20, 160), FRotator(0));

}
void ASYH_MultiPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 서버 위젯
	
	if(HasAuthority())
	{
		CreatePopUpWidget();
		if (!IsLocallyControlled() || GuideWidget != nullptr) return;
		GuideWidget = Cast<UGuide_MainWidget>(CreateWidget(GetWorld(), GuideWidgetClass));
		CheckRequestDistance();
	}
}

// Called when the game starts or when spawned
void ASYH_MultiPlayer::BeginPlay()
{
	Super::BeginPlay();
	UAnimInstance* animinstance = GetMesh()->GetAnimInstance();
	if ( animinstance )
	{
		anim = Cast<USYH_PlayerAnim>(animinstance);
	}
	if (IsLocallyControlled())
	{
		PlayerController = Cast<APlayerController>(Controller);
		if(PlayerController)
		{
			PlayerController->SetShowMouseCursor(false);
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(IMC_Player, 0);
			}
		}
	}
	// 클라이언트 위젯
	if(!HasAuthority())
	{
		CreatePopUpWidget();
		if (!IsLocallyControlled() || GuideWidget != nullptr) return;
		GuideWidget = Cast<UGuide_MainWidget>(CreateWidget(GetWorld(), GuideWidgetClass));
	}
	if(QuizWaitClass)
	{
		QuizWaitWidget = CreateWidget<USYH_QuizWaitWidget>(GetWorld(),QuizWaitClass);
	}
	GameInstance = CastChecked<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(QuizRequestClass)
	{
		QuizRequestWidget = CreateWidget<USYH_QuizSelect>(GetWorld(),QuizRequestClass);
	}
}

// Called every frame
void ASYH_MultiPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsLocallyControlled())
	{
		CheckRequestDistance();
		if(UGameplayStatics::GetCurrentLevelName(GetWorld())!="Room")
		{
			return;
		}
		PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);
	
		if (HitResult.GetActor() != nullptr && HitResult.bBlockingHit)
		{
			OnMyCheckActor();
		}
	}
	
}
void ASYH_MultiPlayer::OnOffGuide(const FInputActionValue& Value)
{
	// 인벤이 화면이 있으면 지우고
	if (GuideWidget->IsInViewport())
	{
		GuideWidget->RemoveFromParent();
	}
	// 인벤이 화면에 없으면 생성
	else
	{
		GuideWidget->AddToViewport();
	}
}
// Called to bind functionality to input
void ASYH_MultiPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASYH_MultiPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASYH_MultiPlayer::Look);

		// Camera
		EnhancedInputComponent->BindAction(IA_Camera, ETriggerEvent::Started, this, &ASYH_MultiPlayer::Camera);
		
		// 도감
		EnhancedInputComponent->BindAction(IA_Guide, ETriggerEvent::Started, this, &ASYH_MultiPlayer::OnOffGuide);

		// quiz
		EnhancedInputComponent->BindAction(IA_Quiz,ETriggerEvent::Started,this,&ASYH_MultiPlayer::Quiz);
		
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASYH_MultiPlayer::ObjectDetect()
{
	// 카메라가 사진찍는 애니메이션을 시작하고 나서 호출
	// linetrace를 사용해 hit된 물체의 GameInstance->TakeItemData(ItemIdx);를 통해 정보를 가져온다
	FHitResult OutHit;
	FVector start = CameraCompFirst->GetComponentLocation();
	FVector end = start + CameraCompFirst->GetForwardVector()*100000;
	float Radius = 60.f;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->SweepSingleByChannel(OutHit,start,end,FQuat::Identity,TraceChannel,FCollisionShape::MakeSphere(Radius),Params);
	if(bHit)
	{
		auto* hitcomp = OutHit.GetComponent();
		if(hitcomp)
		{
			Aluminum = Cast<AAluminum_Object>(OutHit.GetActor());
			// 알루미늄 오브젝트 가져와서
			// GetItemData 호출하기
			if(Aluminum)
			{
				Aluminum->GetItemData();
			}
			else
			{
				bHit = false;
			}
			UE_LOG(LogTemp,Error,TEXT("%s"),*hitcomp->GetName());
		}
		else
		{
			bHit = false;
		}
		UE_LOG(LogTemp,Error,TEXT("here"));
	}
	else
	{
		UE_LOG(LogTemp,Error,TEXT("no bhit"));
	}
}


void ASYH_MultiPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ASYH_MultiPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ASYH_MultiPlayer::Camera(const FInputActionValue& Value)
{
	if(IsLocallyControlled())
	{
		// e키를 누르면 애니메이션이 출력되고 시점을 바꾸고 싶다.
		if ( anim && anim->bIsPlayCameraAnim == true)
		{
			PlayerController->SetShowMouseCursor(true);
			
		}
		// e키를 누르고 카메라가 1인칭 시점인 상태에서 e키를 다시 누르면 원래대로 돌아오게 하고 싶다.
		else if(anim&& anim->bIsPlayCameraAnim == false)
		{
			CameraCompThird->SetActive(true);
			CameraCompFirst->SetActive(false);
			if (GetMesh())
			{
				GetMesh()->SetOwnerNoSee(false);
			}
			// UI도 끄고 싶다.
			if(anim->CameraWidgetClass)
			{
				if(anim->CameraWidget)
				{
					anim->CameraWidget->RemoveFromParent();
					anim->bIsPlayCameraAnim = true;
					PlayerController->SetShowMouseCursor(false);
					FInputModeGameOnly InputMode;
					PlayerController->SetInputMode(InputMode);
				}
			}
		}
	}
}



void ASYH_MultiPlayer::OnMyCheckActor()
{
	AActor* HitActor = HitResult.GetActor();
	if ( interActor != nullptr && HitActor != interActor->GetOwner() )
	{
		if ( interActor->bMouseOnActor )
		{
			interActor->OnCreateNameWidget(false);
		}
	}
	if ( HitActor )
	{
		interActor = HitActor->GetComponentByClass<UKMK_SingleIntaraction>();
		if ( interActor )
		{
			interActor->OnCreateNameWidget(true);
		}

	}
}

void ASYH_MultiPlayer::CreatePopUpWidget()
{
	if(!IsLocallyControlled()) return;
}
// request text를 켜는 함수
void ASYH_MultiPlayer::CallQuizRequestUI_Implementation()
{
	if(QuizWaitWidget)
	{
		QuizWaitWidget->AddToViewport();
		QuizWaitWidget->SetRequestVisibility(true);
	}
}
// request를 보낼 수 있는 거리내에 있으면 UI를 띄우게 함
void ASYH_MultiPlayer::CheckRequestDistance()
{
	// 거리내에 있음을 구분하기 위한 bool 값
	bool bShowUI = false;
	for(TActorIterator<ASYH_MultiPlayer> It(GetWorld());It;++It)
	{
		ASYH_MultiPlayer* OtherPlayer = *It;
		if(OtherPlayer && OtherPlayer!=this)
		{
			float Dist = FVector::Dist(this->GetActorLocation(),OtherPlayer->GetActorLocation());
			if(Dist<RequestUIDistance)
			{
				bShowUI = true;
				OtherPlayer->CallQuizRequestUI();
			}
			
		}
	}


	if (bShowUI)
	{
		CallQuizRequestUI(); // UI를 띄웁니다
	}
	else
	{
		if (QuizWaitWidget)
			QuizWaitWidget->SetRequestVisibility(false); // UI를 숨깁니다.
	}
}

void ASYH_MultiPlayer::Quiz(const FInputActionValue& Value)
{
	// F키를 누르면 인스턴스에 요청을 보내 클라이언트에게 전달하고 싶다.
	// F키를 눌러 line trace를 사용하여 범위내에 있는 다른 플레이어에게 퀴즈 요청을 보내고 싶다.
	FHitResult OutHit;
	FVector start = CameraCompThird->GetComponentLocation();
	FVector end = start + CameraCompThird->GetForwardVector()*10000;
	ECollisionChannel TraceChannel = ECC_Visibility;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	float Radius = 100.f;
	if(GetWorld()->SweepSingleByChannel(OutHit,start,end,FQuat::Identity,TraceChannel,FCollisionShape::MakeSphere(Radius),Params))
	{
		if(HasAuthority())
		{
			// 서버일 경우 서버에서 바로 처리
			HandleQuizRequest(OutHit.GetActor());
		}
		else
		{
			// 클라이언트일 경우 서버에 요청
			SendQuizRequest(start,end,Radius);
			
		}
	}
}

void ASYH_MultiPlayer::SendQuizRequest_Implementation(FVector start, FVector end, float radius)
{
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (GetWorld()->SweepSingleByChannel(OutHit,start,end,FQuat::Identity,ECC_Visibility,FCollisionShape::MakeSphere(radius),Params))
	{
		// 퀴즈 요청 처리
		HandleQuizRequest(OutHit.GetActor());
	}
}
bool ASYH_MultiPlayer::SendQuizRequest_Validate(FVector start, FVector end, float radius)
{
	// 유효성 검사
	return true;
}

void ASYH_MultiPlayer::HandleQuizRequest(AActor* TargetActor)
{
	// 퀴즈 요청을 보내어 hit이 된 player가 있으면 게임 인스턴스로 요청을 보내서 클라이언트에게 전달하게 하고 싶다.
	if (GameInstance)
	{
		// 클라이언트에서 위젯을 표시
		if (IsLocallyControlled())
		{
			ShowQuizRequestUI();
		}
        
		// 서버에서 게임 인스턴스로 요청 전달
		if (HasAuthority())
		{
			GameInstance->HandleQuizRequest(this, TargetActor);
		}
	}
}

void ASYH_MultiPlayer::ShowQuizRequestUI_Implementation()
{
	if(QuizRequestWidget)
	{
		QuizRequestWidget->AddToViewport();
	}
}