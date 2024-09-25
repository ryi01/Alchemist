// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/SectionActor.h"
#include "KMK/PlayerInteractionComponent.h"

// Sets default values
ASectionActor::ASectionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);
}

// Called when the game starts or when spawned
void ASectionActor::BeginPlay()
{
	Super::BeginPlay();
    if ( comp )
    {

        // OnComponentHit 델리게이트에 함수 바인딩
        comp->OnComponentHit.AddDynamic(this,&ASectionActor::OnHit);
    }

}

// Called every frame
void ASectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASectionActor::SetCollisionMesh()
{
    if ( comp )
    {
        comp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Destroy();
    }
}

void ASectionActor::OnHit(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit)
{
    auto* player = OtherActor->FindComponentByClass<UPlayerInteractionComponent>();
    if ( player )
    {
        if ( player->collectionTag.Contains(Tags[ 1 ]) )
        {
            GEngine->AddOnScreenDebugMessage(3,5,FColor::White,FString::Printf(TEXT("slkdajfld")));
        }
    }
}


