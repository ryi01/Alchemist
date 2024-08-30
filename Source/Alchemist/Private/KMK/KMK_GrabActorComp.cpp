// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_GrabActorComp.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
UKMK_GrabActorComp::UKMK_GrabActorComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKMK_GrabActorComp::BeginPlay()
{
	Super::BeginPlay();

	auto* box = GetOwner()->FindComponentByClass<UBoxComponent>();
	check(box);
	if ( box )
	{
		box->OnComponentBeginOverlap.AddDynamic(this, &UKMK_GrabActorComp::BeginOverlap);
	}
	
}


// Called every frame
void UKMK_GrabActorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UKMK_GrabActorComp::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 액터 중에 tag = element인것만 넣기
	if ( OtherActor->ActorHasTag(TEXT("element")) )
	{
		FString checkTagName = OtherActor->Tags[1].ToString();
		if(checkTagName == "" ) return;
		if ( ElementArray.Contains(checkTagName) ) ElementArray[ checkTagName ]++;
		else ElementArray.Add(checkTagName,1);

		OtherActor->Destroy();
	}

}

