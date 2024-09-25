// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SectionActor.generated.h"

UCLASS()
class ALCHEMIST_API ASectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASectionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool isCollisionOn = false;
	UPROPERTY(BlueprintReadWrite)
	class UStaticMeshComponent* comp;

	FName myTag;

	UFUNCTION()
	void SetCollisionMesh();

    // Hit 이벤트 처리 함수
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
               UPrimitiveComponent* OtherComp, FVector NormalImpulse, 
               const FHitResult& Hit);
};
