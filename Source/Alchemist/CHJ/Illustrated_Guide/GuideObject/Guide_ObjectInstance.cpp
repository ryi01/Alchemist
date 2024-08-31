// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Illustrated_Guide/GuideObject/Guide_ObjectInstance.h"

UGuide_ObjectInstance::UGuide_ObjectInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UGuide_ObjectInstance::Init(int32 InitObjectID)
{
	if(InitObjectID <= 0) return;

	IG_ObjectID = InitObjectID;

}
