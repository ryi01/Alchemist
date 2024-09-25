﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/SectionActor.h"

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

