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
#include "KMK/KMK_PopUpWidget.h"
#include "Blueprint/UserWidget.h"
#include "SYH/CameraWidget.h"
#include "SYH/SYH_PlayerAnim.h"
#include "KMK/KMK_SingleIntaraction.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Widget/Guide_MainWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "KMK/KMK_DeskComponent.h"
#include "KMK/KMK_PlayerMouse.h"

DEFINE_LOG_CATEGORY(LogTemplate);

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
	

}

void ASYH_Player::BeginPlay()
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
			PlayerController->SetShowMouseCursor(true);
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
	}
}

void ASYH_Player::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 서버 위젯
	if(HasAuthority())
	{
		CreatePopUpWidget();
	}
}

void ASYH_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsLocallyControlled())
	{
		if(UGameplayStatics::GetCurrentLevelName(GetWorld())!="Room1")
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

// Input


void ASYH_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ASYH_Player::Move);

		// Looking
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ASYH_Player::Look);

		// 마우스 클릭
		EnhancedInputComponent->BindAction(IA_Mouse, ETriggerEvent::Started, this, &ASYH_Player::OnClickedLeft);
	
	}
	else
	{
		UE_LOG(LogTemplate, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
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

void ASYH_Player::OnClickedLeft(const FInputActionValue& Value)
{
	
	if ( HitResult.bBlockingHit )
	{
		AActor* HitActor = HitResult.GetActor();
		if ( HitActor )
		{
			auto* desk = HitActor->GetComponentByClass<UKMK_DeskComponent>();
			auto* mouse = HitActor->GetComponentByClass<UKMK_PlayerMouse>();
			if ( HitActor->ActorHasTag("Desk") )
			{
				if ( desk )
				{
					DeskActor = HitActor;
					mouse->isDesk = true;
					mouse->handle = GetComponentByClass<UPhysicsHandleComponent>();
					desk->ChangeMyCamera(true);
				}
				return;
			}
			count = 0;
			auto* actorClass = HitActor->GetComponentByClass<UKMK_SingleIntaraction>();
			if ( actorClass && bCreateWidget )
			{
				actorClass->CreatePlayerWidget(true,0, PlayerController);
				//PlayerController->SetPause(true);
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

void ASYH_Player::CreatePopUpWidget()
{
	if(!IsLocallyControlled()) return;
	bCreateWidget = true;
}

