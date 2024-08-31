// Fill out your copyright notice in the Description page of Project Settings.


#include "SYH/SYH_MultiPlayer.h"
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
#include "KMK_SingleIntaraction.h"
#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Aluminum_Object.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_MainWidget.h"
#include "Kismet/GameplayStatics.h"
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
	
}

// Called every frame
void ASYH_MultiPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsLocallyControlled())
	{
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
	bCreateWidget = true;
}



