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
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "SYH/SYH_MenuWidget.h"
#include "SYH/SYH_QuizSelect.h"
#include "SYH/SYH_QuizWaitWidget.h"
#include "SYH/SYH_QuizWidget.h"
#include "SYH/SYH_QuizWidgetResult.h"
#include "KMK/PlayerInteractionComponent.h"
#include "KMK/MissionWidget.h"
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

	interactionComp = CreateDefaultSubobject<UPlayerInteractionComponent>(TEXT("Interaction"));
	CameraComp= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(GetMesh(),TEXT("RightHand"));
	CameraComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh>CameraCompMesh(TEXT("/Script/Engine.StaticMesh'/Game/Project/SYH/Camera/camera.camera'"));
	if(CameraCompMesh.Succeeded())
	{
		CameraComp->SetStaticMesh(CameraCompMesh.Object);
		
	}
}
void ASYH_MultiPlayer::PossessedBy(AController* NewController) // server에서만 불림
{
	Super::PossessedBy(NewController);

	// 서버일 때의 위젯 생성
	if(HasAuthority())
	{
		if (!IsLocallyControlled() || GuideWidget != nullptr || QuizWaitWidget != nullptr || QuizSelectWidget != nullptr || QuizWidget != nullptr ||QuizResultWidget != nullptr) return;
		GuideWidget = Cast<UGuide_MainWidget>(CreateWidget(GetWorld(), GuideWidgetClass));
		QuizWaitWidget = Cast<USYH_QuizWaitWidget>(CreateWidget(GetWorld(), QuizWaitClass));
		if(QuizWaitWidget) QuizWaitWidget->AddToViewport();
		QuizSelectWidget = Cast<USYH_QuizSelect>(CreateWidget(GetWorld(), QuizSelectClass));
		QuizWidget = Cast<USYH_QuizWidget>(CreateWidget(GetWorld(),QuizClass));
		QuizResultWidget = Cast<USYH_QuizWidgetResult>(CreateWidget(GetWorld(),QuizResultClass));
		MenuWidget = Cast<USYH_MenuWidget>(CreateWidget(GetWorld(),MenuClass));
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
		// 내가 컨트롤하는 즉, 내가 플레이하고 있는 캐릭터
	{
		PlayerController = Cast<APlayerController>(Controller);
		if(PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(IMC_Player, 0);
			}
		}
	}
	// 클라이언트일 때의 위젯 생성
	if(!HasAuthority())
	{
		if (!IsLocallyControlled() || GuideWidget != nullptr || QuizWaitWidget != nullptr || QuizSelectWidget != nullptr || QuizWidget != nullptr ||QuizResultWidget != nullptr) return;
		GuideWidget = Cast<UGuide_MainWidget>(CreateWidget(GetWorld(), GuideWidgetClass));
		QuizWaitWidget = Cast<USYH_QuizWaitWidget>(CreateWidget(GetWorld(), QuizWaitClass));
		if(QuizWaitWidget) QuizWaitWidget->AddToViewport();
		UE_LOG(LogTemp,Error,TEXT("client"));
		QuizSelectWidget = Cast<USYH_QuizSelect>(CreateWidget(GetWorld(), QuizSelectClass));
		QuizWidget = Cast<USYH_QuizWidget>(CreateWidget(GetWorld(),QuizClass));
		QuizResultWidget = Cast<USYH_QuizWidgetResult>(CreateWidget(GetWorld(),QuizResultClass));
		MenuWidget = Cast<USYH_MenuWidget>(CreateWidget(GetWorld(),MenuClass));
	}
	GameInstance = CastChecked<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

// Called every frame
void ASYH_MultiPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(isWidget) return;
	// 캐릭터 머리 위에 bool 값을 출력
	FString BoolText = InQuiz ? TEXT("True") : TEXT("False");

	// 텍스트를 캐릭터의 위치 + 오프셋(머리 위)에 표시
	FVector TextLocation = GetActorLocation() + FVector(0, 0, 100);  // 캐릭터 머리 위 100 유닛

	// DrawDebugString을 사용해 텍스트를 표시
	DrawDebugString(GetWorld(), TextLocation, FString::Printf(TEXT("InQuiz: %s"), *BoolText), nullptr, FColor::Green, 0.0f, true);

	if (QuizWaitWidget != nullptr && QuizSelectWidget != nullptr && QuizWidget != nullptr && QuizResultWidget != nullptr)
	{
		if (IsLocallyControlled())
		{
			if (!InQuiz)
			{
				CheckDist(true);
			}
			else
			{
				interactionComp->DeleteMainWidget();
			}
		}
	}
	if(IsLocallyControlled())
	{
		if(UGameplayStatics::GetCurrentLevelName(GetWorld())!=TEXT("SYHLevel"))
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
// request를 보낼 수 있는 거리내에 있으면 UI를 띄우게 함

void ASYH_MultiPlayer::CheckDist(bool bCheck)
{
	if(QuizSelectWidget->IsInViewport() || QuizWidget->IsInViewport() || QuizResultWidget->IsInViewport() || QuizWaitWidget->Reject->IsInViewport() || QuizWaitWidget->Wait->IsInViewport()) return;
	// 거리내에 있음을 구분하기 위한 bool 값
	bool bShowUI = false;
	for(TActorIterator<ASYH_MultiPlayer> It(GetWorld());It;++It)
	{
		TargetPlayer = *It;
		if(TargetPlayer && TargetPlayer!=this)
		{
			float Dist = FVector::Dist(this->GetActorLocation(),TargetPlayer->GetActorLocation());
			if(Dist<RequestUIDistance)
			{
				bShowUI = true;
				break;
			}
		}
	}
	if (bShowUI && bCheck && TargetPlayer->InQuiz == false)
	{
		if ( interactionComp->missionWidget ) interactionComp->DeleteMainWidget();
		ClientRPC_CallFKey(); // UI를 띄움 (client의 UI까지)
	}
	else
	{
		if (QuizWaitWidget)
			QuizWaitWidget->SetRequestVisibility(false); // UI를 숨깁니다.
	}
	if(!bShowUI ) interactionComp->CreateMainWidget();
}

// request text를 켜는 함수
// server가 띄우는 UI가 client에도 뜨게 하기 위해 필요함
// 일반함수로 처리도 가능하지만 RPC로 처리해 모든 클라이언트들이 같은 결과를 내도록 함.
void ASYH_MultiPlayer::ClientRPC_CallFKey_Implementation()
{
	if(QuizWaitWidget)
	{
		QuizWaitWidget->SetRequestVisibility(true);
	}
}

void ASYH_MultiPlayer::OnOffGuide(const FInputActionValue& Value)
{
	// 인벤이 화면이 있으면 지우고
	if (GuideWidget->IsInViewport())
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
		GuideWidget->RemoveFromParent();
	}
	// 인벤이 화면에 없으면 생성
	else
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeGameOnly());
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
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &ASYH_MultiPlayer::MyJump);

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
		// esc키
		EnhancedInputComponent->BindAction(IA_Menu,ETriggerEvent::Started,this,&ASYH_MultiPlayer::Menu);
		interactionComp->SetupInputBinding(EnhancedInputComponent);
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
		if(QuizWaitWidget->Request) QuizWaitWidget->SetRequestVisibility(false);
		InQuiz = true; // 사진을 찍고 있으면 F키를 누르라는 Ui가 뜨지 않게함
		// e키를 누르면 애니메이션이 출력되고 시점을 바꾸고 싶다.
		if ( anim && anim->bIsPlayCameraAnim == true)
		{
			PlayerController->SetShowMouseCursor(true);
		}
		// e키를 누르고 카메라가 1인칭 시점인 상태에서 e키를 다시 누르면 원래대로 돌아오게 하고 싶다.
		else if(anim&& anim->bIsPlayCameraAnim == false)
		{
			InQuiz = false;
			CameraCompThird->SetActive(true);
			CameraCompFirst->SetActive(false);
			if (GetMesh())
			{
				CameraComp->SetVisibility(true,true);
				GetMesh()->SetOwnerNoSee(false);
			}
			// UI도 끄고 싶다.
			if(anim->CameraWidgetClass)
			{
				if(anim->CameraWidget)
				{
					anim->CameraWidget->RemoveFromParent();
					anim->bIsPlayCameraAnim = true;
					if(APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager)
					{
						CameraManager->SetFOV(0);
					}
					PlayerController->SetShowMouseCursor(false);
					PlayerController->SetInputMode(FInputModeGameOnly());
				}
			}
		}
	}
}

void ASYH_MultiPlayer::MyJump(const FInputActionValue& Value)
{

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



void ASYH_MultiPlayer::Server_Quiz()
{
	FHitResult OutHit;
	FVector Start = CameraCompThird->GetComponentLocation();
	FVector End = Start + CameraCompThird->GetForwardVector() * 1000.0f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(10.f), Params);
	if (bHit)
	{
		TargetPlayer = Cast<ASYH_MultiPlayer>(OutHit.GetActor());
		if (TargetPlayer)
		{
			me = this;
			this->TargetPlayer = TargetPlayer;
			TargetPlayer->TargetPlayer = me;
			InQuiz = true;
			TargetPlayer->InQuiz = true;
			UE_LOG(LogTemp, Warning, TEXT("Server_Quiz: TargetPlayer set to %s"), *TargetPlayer->GetName());
			UE_LOG(LogTemp, Warning, TEXT("Server_Quiz: TargetPlayer set to %s"), *me->GetName());
			// 요청을 받은 플레이어에게 UI를 띄우도록 서버에서 클라이언트로 요청
			this->ClientRPC_ShowQuizWait();
			TargetPlayer->ClientRPC_ShowQuizSelect();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Server_Quiz: TargetPlayer is null after Sweep Trace"));
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server_Quiz: No player hit by Sweep Trace"));
	}
}

void ASYH_MultiPlayer::Quiz(const FInputActionValue& Value)
{
	if (!QuizWaitWidget || !QuizWaitWidget->IsInViewport() || QuizSelectWidget->IsInViewport() || QuizWidget->IsInViewport() || QuizResultWidget->IsInViewport()) return;
	if(TargetPlayer&& TargetPlayer->InQuiz && TargetPlayer->InQuiz == true) return;
	// F키를 누르면 요청을 보낸 사람과 받는 사람의 UI를 다르게 하고 싶다
	// F키를 눌러 sweep trace를 사용하여 범위내에 있는 다른 플레이어에게 퀴즈 요청을 보내고 싶다.
	if (HasAuthority())  // 서버에서만 이 로직이 실행됨
	{
		Server_Quiz();  // 서버에서 퀴즈 요청을 처리함
	}
	else
	{
		ServerRPC_Quiz();  // 클라이언트가 서버에 퀴즈 요청을 보냄
	}
	
}

void ASYH_MultiPlayer::Menu(const FInputActionValue& Value)
{
	if(MenuWidget)
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly());
		MenuWidget->AddToViewport();
	}
}

void ASYH_MultiPlayer::ClientRPC_ShowQuizSelect_Implementation()
{
	if(QuizWaitWidget)
	{
		QuizWaitWidget->SetRequestVisibility(false);// 기존 UI를 숨김
	}
	if(QuizSelectWidget)
	{
		InQuiz = true;
		QuizSelectWidget->AddToViewport();
	}
}
void ASYH_MultiPlayer::ClientRPC_ShowQuizWait_Implementation()
{
	if (QuizWaitWidget)
	{
		QuizWaitWidget->SetRequestVisibility(false); // 기존 UI를 숨김
		QuizWaitWidget->SetWaitVisibility(true);
	}
}

void ASYH_MultiPlayer::ServerRPC_Quiz_Implementation()
{
	InQuiz = true;
	Server_Quiz();
}

bool ASYH_MultiPlayer::ServerRPC_Quiz_Validate()
{
	return true;
}

void ASYH_MultiPlayer::ClientRPC_ShowQuizReject_Implementation()
{
	if(QuizWaitWidget)
	{
		QuizWaitWidget->SetWaitVisibility(false);
		QuizWaitWidget->SetRejectVisibility(true);
		GetWorld()->GetTimerManager().SetTimer(Timer,this,&ASYH_MultiPlayer::HideQuizReject,3.0f,false);
	}
}
void ASYH_MultiPlayer::HideQuizReject()
{
	// 요청을 받은 사람
	this->InQuiz = false;
	this->TargetPlayer->InQuiz = false;
	if(QuizWaitWidget)
	{
		QuizWaitWidget->SetRejectVisibility(false);
		interactionComp->CreateMainWidget();
	}
}
void ASYH_MultiPlayer::ClientRPC_ShowQuiz_Implementation()
{
	if(QuizWaitWidget)
	{
		QuizWaitWidget->SetWaitVisibility(false);
	}
	if(QuizWidget)
	{
		QuizWidget->AddToViewport();
	}
}
void ASYH_MultiPlayer::ServerRPC_AcceptQuiz_Implementation()
{
	this->ClientRPC_ShowQuiz();
	if(TargetPlayer)
	{
		TargetPlayer->ClientRPC_ShowQuiz();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRPC_AcceptQuiz_Implementation"));
	}
}

bool ASYH_MultiPlayer::ServerRPC_AcceptQuiz_Validate()
{
	return true;

}

void ASYH_MultiPlayer::ServerRPC_RejectQuiz_Implementation()
{
	//  TargetPlayer 요청을 하고 대기중인 플레이어
	if(TargetPlayer)
	{
		TargetPlayer->ClientRPC_ShowQuizReject();
		GetWorld()->GetTimerManager().SetTimer(Timer,this,&ASYH_MultiPlayer::HideQuizReject,3.0f,false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ServerRPC_RejectQuiz_Implementation"));
	}
}

bool ASYH_MultiPlayer::ServerRPC_RejectQuiz_Validate()
{
	return true;
}


void ASYH_MultiPlayer::Server_Compare()
{
	for (TActorIterator<ASYH_MultiPlayer> It(GetWorld()); It; ++It)
	{
		ASYH_MultiPlayer* OtherPlayer = *It;
		if (OtherPlayer && OtherPlayer != this)
		{

			if(OtherPlayer->RightCount == -1)
			{
				ClientRPC_ShowWaitResult();
			}
			else // 둘다 퀴즈를 끝낸 상태
			{
				if (RightCount > OtherPlayer->RightCount)
				{
					UE_LOG(LogTemp,Error,TEXT("win"));
					UE_LOG(LogTemp,Error,TEXT("count : %d"),RightCount);
					UE_LOG(LogTemp,Error,TEXT("count : %d"),OtherPlayer->RightCount);
					// 이 플레이어가 승리
					ClientRPC_ShowWinResult();
					OtherPlayer->ClientRPC_ShowLoseResult();
				}
				else if (RightCount < OtherPlayer->RightCount)
				{
					UE_LOG(LogTemp,Error,TEXT("lose"));
					UE_LOG(LogTemp,Error,TEXT("count : %d"),RightCount);
					UE_LOG(LogTemp,Error,TEXT("count : %d"),OtherPlayer->RightCount);
					// 이 플레이어가 패배
					ClientRPC_ShowLoseResult();
					OtherPlayer->ClientRPC_ShowWinResult();
				}
				else
				{
					UE_LOG(LogTemp,Error,TEXT("same"));
					UE_LOG(LogTemp,Error,TEXT("count : %d"),RightCount);
					UE_LOG(LogTemp,Error,TEXT("count : %d"),OtherPlayer->RightCount);
					// 동점
					ClientRPC_ShowSameResult();
					OtherPlayer->ClientRPC_ShowSameResult();
				}
			}
		}
	}
}

void ASYH_MultiPlayer::ServerRPC_SendRightCount_Implementation(int32 Count)
{
	RightCount = Count;
	Server_Compare();
}

bool ASYH_MultiPlayer::ServerRPC_SendRightCount_Validate(int32 Count)
{
	return true;
}


void ASYH_MultiPlayer::ClientRPC_ShowWaitResult_Implementation()
{
	QuizResultWidget->AddToViewport();
	QuizResultWidget->SetWaitVisibility(true);
}

void ASYH_MultiPlayer::SetShowMyMouse(bool isActive)
{
	isActive = isWidget;
	PlayerController->SetShowMouseCursor(isActive);
}

void ASYH_MultiPlayer::ClientRPC_ShowSameResult_Implementation()
{
	if(QuizResultWidget->IsInViewport())
	{
		QuizResultWidget->SetWaitVisibility(false);
		QuizResultWidget->SetSameVisibility(true);
	}
	else
	{
		QuizResultWidget->AddToViewport();
		QuizResultWidget->SetSameVisibility(true);
	}
	RightCount = -1;
	TargetPlayer->RightCount = -1;
}

void ASYH_MultiPlayer::ClientRPC_ShowLoseResult_Implementation()
{
	if(QuizResultWidget->IsInViewport())
	{
		QuizResultWidget->SetWaitVisibility(false);
		QuizResultWidget->SetLoseVisibility(true);
	}
	else
	{
		QuizResultWidget->AddToViewport();
		QuizResultWidget->SetLoseVisibility(true);
	}
	RightCount = -1;
	TargetPlayer->RightCount = -1;
}

void ASYH_MultiPlayer::ClientRPC_ShowWinResult_Implementation()
{
	if(QuizResultWidget->IsInViewport())
	{
		QuizResultWidget->SetWaitVisibility(false);
		QuizResultWidget->SetWinVisibility(true);
	}
	else
	{
		QuizResultWidget->AddToViewport();
		QuizResultWidget->SetWinVisibility(true);
	}
	RightCount = -1;
	TargetPlayer->RightCount = -1;
}
void ASYH_MultiPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASYH_MultiPlayer,IsWin);
	DOREPLIFETIME(ASYH_MultiPlayer,IsLose);
	DOREPLIFETIME(ASYH_MultiPlayer,InQuiz);
}
