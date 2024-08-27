// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GuideComponent.generated.h"

USTRUCT(BlueprintType)
struct FElementDatas : public FTableRowBase
{
	GENERATED_BODY()
public:
	//UPROPERTY(EditAnywhere)
	//EItemType type;

	UPROPERTY(EditAnywhere)
	UTexture2D* Element_thumbnail;

	UPROPERTY(EditAnywhere)
	FString Element_name;
}; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMIST_API UGuideComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGuideComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//UPROPERTY(EditAnywhere)
	//TArray<FItemDatas> myItems;
};
