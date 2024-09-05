// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_GrabActorComp.generated.h"

// 항아리 엑터에 넣을 컴포넌트
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UKMK_GrabActorComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_GrabActorComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TMap<FString, int32> ElementArray;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> elementFact;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool isCreate = false;
	UPROPERTY()
	AActor* newActor; 
	UFUNCTION()
	void CreateElementSucced(FString tagName);
	UPROPERTY()
	int count = 0;

	UFUNCTION()
	// 오버랩 감지를 위한 함수 선언
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
