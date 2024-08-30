// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Aluminum_Object.h"

#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAluminum_Object::AAluminum_Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

}

// Called when the game starts or when spawned
void AAluminum_Object::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = CastChecked<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAluminum_Object::OnOverlapBegin);
	
}

// Called every frame
void AAluminum_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAluminum_Object::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* OtherPlayer = Cast<ACharacter>(OtherActor);
	//캐릭터가 충돌 되었다면 가이드 컴포넌트의 데이터를 set 한다.
	if(OtherPlayer)
	{	
		GameInstance->TakeItemData(ItemIdx);
	}
}


