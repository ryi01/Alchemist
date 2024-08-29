// Copyright Epic Games, Inc. All Rights Reserved.

#include "SYH/SYH_Player.h"
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
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

// ATP_ThirdPersonCharacter

ASYH_Player::ASYH_Player()
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

void ASYH_Player::BeginPlay()
{
	Super::BeginPlay();
	
	UAnimInstance* animinstance = GetMesh()->GetAnimInstance();
	if ( animinstance )
	{
		anim = Cast<USYH_PlayerAnim>(animinstance);
	}
	player = Cast<APlayerController>(Controller);
	if ( player )
	{
		player->SetShowMouseCursor(true);
	}
}


void ASYH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(UGameplayStatics::GetCurrentLevelName(GetWorld())!="Room")
	{
		return;
	}
	player->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

    if ( HitResult.GetActor() != nullptr && HitResult.bBlockingHit )
    {
		OnMyCheckActor();
    }
	
}

// Input

void ASYH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Player, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASYH_Player::Move);

		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASYH_Player::Look);

		// Camera
		EnhancedInputComponent->BindAction(IA_Camera, ETriggerEvent::Started, this, &ASYH_Player::Camera);

		// 마우스 클릭
		EnhancedInputComponent->BindAction(IA_Mouse, ETriggerEvent::Started, this, &ASYH_Player::OnClickedLeft);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ASYH_Player::Move(const FInputActionValue& Value)
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

void ASYH_Player::Look(const FInputActionValue& Value)
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

void ASYH_Player::Camera(const FInputActionValue& Value)
{
	// e키를 누르면 애니메이션이 출력되고 시점을 바꾸고 싶다.
	if ( anim && anim->bIsPlayCameraAnim == true)
	{
		// 카메라를 드는 애니메이션이 출력되도록 bool 값을 true로 설정
		anim->bIsCamera = true;
	}
	// e키를 누르고 카메라가 1인칭 시점인 상태에서 e키를 다시 누르면 원래대로 돌아오게 하고 싶다.
	else if(anim&& anim->bIsPlayCameraAnim == false)
	{
		CameraCompThird->SetActive(true);
		CameraCompFirst->SetActive(false);
		// UI도 끄고 싶다.
		if(anim->CameraWidgetClass)
		{
			if(anim->CameraWidget)
			{
				anim->CameraWidget->RemoveFromParent();
				anim->bIsPlayCameraAnim = true;
			}
		}
	}
}

void ASYH_Player::OnClickedLeft(const FInputActionValue& Value)
{
	if ( HitResult.bBlockingHit )
	{
		AActor* HitActor = HitResult.GetActor();
		if ( HitActor )
		{
			count = 0;
			auto* actorClass = HitActor->GetComponentByClass<UKMK_SingleIntaraction>();
			if ( actorClass )
			{
				// actorClass->CreatePlayerWidget(true);
				player->SetPause(true);
				
			}
		}
	}

}

void ASYH_Player::OnMyCheckActor()
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

