// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Aluminum_Object.generated.h"

UCLASS()
class ALCHEMIST_API AAluminum_Object : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAluminum_Object();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UGuideComponent* GuideComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;
	UFUNCTION(BlueprintCallable)
	void GetItem();
	
};
