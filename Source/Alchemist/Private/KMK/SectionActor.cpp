// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/SectionActor.h"
#include "KMK/PlayerInteractionComponent.h"
#include "SYH/SYH_MultiPlayer.h"

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
    if ( !comp )
    {
        comp = FindComponentByClass<UStaticMeshComponent>();
    }

    // 필요할 경우 추가 설정
    if ( comp )
    {
        comp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}

// Called every frame
void ASectionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ASectionActor::MultiRPCPlayerIgnore_Implementation(class AActor* player)
{
    if ( comp && player )
    {
        comp->IgnoreActorWhenMoving(player,true);
    }
}

void ASectionActor::SetPlayerIgnore(class AActor* player)
{
    if ( HasAuthority() ) // 서버에서만 실행
    {
        // 서버에서 Multicast RPC 호출하여 모든 클라이언트에게 전파
        MultiRPCPlayerIgnore(player);
    }
}



void ASectionActor::ServerRPCPassSection_Implementation(class AActor* player)
{
    SetPlayerIgnore(player);
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


