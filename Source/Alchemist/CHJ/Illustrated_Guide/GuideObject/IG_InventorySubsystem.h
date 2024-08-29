// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "IG_InventorySubsystem.generated.h"

class UGuide_ObjectInstance;
/**아이템 매니저
 * 아이템 인스턴스들을 동적으로 들고있는다.
 * 원본 데이터는 서브시스템이 들고있고 UI를 그릴때 GetItmes를 참고해서 여기있는 데이터를 꺼내서 그려라
 */
UCLASS()
class ALCHEMIST_API UIG_InventorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override; //초기화 함수
	virtual void Deinitialize() override; //삭제될때 호출
	// 아이템의 참조값을 리턴해서 밖으로 갖고 올 수 있게 const로 꺼내서 나중에 UI로 그려준다.
	const TArray<TObjectPtr<UGuide_ObjectInstance>>& GetItems() {return Items;} 
	
	void AddDefaultItems(); // 가라 아이템
protected:
	TArray<TObjectPtr<UGuide_ObjectInstance>> Items;
};
