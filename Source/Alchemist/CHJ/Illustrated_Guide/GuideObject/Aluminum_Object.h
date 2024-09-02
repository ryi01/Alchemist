// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Alchemist/CHJ/Illustrated_Guide/Guide_Component/GuideComponent.h"
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
	class UBoxComponent* BoxComponent;

	//콜리전 함수
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(EditAnywhere)
	int32 ItemIdx = 0; // 알루미늄 데이터 인덱스 0
	
	class UGuide_GameInstance* GameInstance;
	
	void GetItemData();
};
