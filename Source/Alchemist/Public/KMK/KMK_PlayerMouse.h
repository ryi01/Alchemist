// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KMK_PlayerMouse.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UKMK_PlayerMouse : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKMK_PlayerMouse();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	class UPhysicsHandleComponent* handle;
	UPROPERTY()
	class APlayerController* me;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRay = false;
	UPROPERTY()
	class UPrimitiveComponent* outHitComp;
	UPROPERTY(EditAnywhere)
	float RayDis = 500;
	UPROPERTY(EditAnywhere)
	float distance = 250;

	UPROPERTY(EditAnywhere)
	AActor* HttpActor;

	TArray<FVector> mousePos;
	int count = 0;

	UFUNCTION()
	TArray<FVector> GetMouseWorldDirection();

	UFUNCTION()
	void OnMyGrabComp();
	UFUNCTION()
	void OnMyPutComp(class UPrimitiveComponent* comp);
	// 새로운 엑터 생성하는 함수
	UFUNCTION()
	void CopyNewActor(AActor* hitActor, FVector grabPos);
};
