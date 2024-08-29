// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Aluminum_Object.h"

#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAluminum_Object::AAluminum_Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	GuideComponent = CreateDefaultSubobject<UGuideComponent>(TEXT("GuideComponent"));

}

// Called when the game starts or when spawned
void AAluminum_Object::BeginPlay()
{
	Super::BeginPlay();

	GetItem();
}

// Called every frame
void AAluminum_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAluminum_Object::GetItem()
{
	UGuide_GameInstance* GameInstance = Cast<UGuide_GameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if(GameInstance)
	{
		GuideComponent->myItems.Add(GameInstance->DefineItem[0]);
	}
}

