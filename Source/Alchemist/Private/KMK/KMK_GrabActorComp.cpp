// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_GrabActorComp.h"
#include "Components/BoxComponent.h"
#include "KMK/KMK_SingleIntaraction.h"
#include "KMK/KMK_ElementGameActor.h"
#include "Components/WidgetComponent.h"

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
	auto* wid = GetOwner()->GetComponentByClass<UWidgetComponent>();
	if ( wid )
	{
		wid->GetWidget()->SetVisibility(ESlateVisibility::Hidden);
	}
}


// Called every frame
void UKMK_GrabActorComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UKMK_GrabActorComp::CreateElementSucced(FString tagName, const FString& text)
{
	if ( !isCreate )
	{
		isCreate = true;
		newActor = GetWorld()->SpawnActor<AActor>(elementFact,GetOwner()->GetTargetLocation(),FRotator::ZeroRotator);
		newActor->Tags.Add(FName(*tagName));
		auto* newText = newActor->GetComponentByClass<UKMK_ElementGameActor>();
		if ( newText )
		{
			newText->SetTextWidget(text);
			newText->OnCreateWidget(false);
		}
		count++;
	}
}

void UKMK_GrabActorComp::CreateElementFailed()
{
	if ( httpActor != nullptr && cnt < 1 )
	{
		httpActor->ReqRecommandEle(json);
		cnt++;
	}
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

