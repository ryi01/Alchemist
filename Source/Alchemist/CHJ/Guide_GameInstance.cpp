// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Guide_GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "SYH/SYH_QuizWaitWidget.h"

UGuide_GameInstance::UGuide_GameInstance()
{
	//데이터 테이블 붙이기
	 ConstructorHelpers::FObjectFinder<UDataTable> tempDT(TEXT("/Script/Engine.DataTable'/Game/Project/CHJ/Illerstrated_Guide/Data/DT_ElementData.DT_ElementData'"));
	 if (tempDT.Succeeded())
	 {	
	 	UDataTable* dataTable = tempDT.Object;
	
	 	// GetRowNames는 전체 열을 가져옴
	 	TArray<FName> rowNames = dataTable->GetRowNames();
	 	// 열을 순차적으로 추가한다
	 	for (int i = 0; i < rowNames.Num(); i++)
	 	{
	 		// 열을 찾겠다 -> rownames의 i번째 // ContextString : 뭘 포함하고 있는지
	 		FElementDatas* itemData = dataTable->FindRow<FElementDatas>(rowNames[i], TEXT(""));
	 		// 찾은거 defineItem에 넣겠다 (defineItem은 포인터로 선언하지 않아 itemData 주소값으로 값을 가져와야함)
	 		DefineItem.Add(*itemData);
	 	}
	 }
}



void UGuide_GameInstance::Init()
{
	Super::Init();
	//flag를 PictureItems 만큼 생성 / 디폴트 값은 false.
	PictureItemFlags.Init(false, SlotsIndex);

	//델리게이트 바인딩
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this,&UGuide_GameInstance::OnCreateSessionComplete);
			OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this,&UGuide_GameInstance::OnFindSessionComplete);
			OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this,&UGuide_GameInstance::OnJoinSessionComplete);
			OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this,&UGuide_GameInstance::OnDestroySessionComplete);

			SessionInterface->OnCreateSessionCompleteDelegates.Add(OnCreateSessionCompleteDelegate);
			SessionInterface->OnFindSessionsCompleteDelegates.Add(OnFindSessionsCompleteDelegate);
			SessionInterface->OnJoinSessionCompleteDelegates.Add(OnJoinSessionCompleteDelegate);
			SessionInterface->OnDestroySessionCompleteDelegates.Add(OnDestroySessionCompleteDelegate);
		}
	}
}

void UGuide_GameInstance::TakeItemData(int itemIdx)
{	// DefineItem의 데이터 추가
	PictureItems.Add(DefineItem[itemIdx]);
	// 중복 체크 필요
	 if(PictureItems.IsValidIndex(itemIdx)) // 아이템 인덱스가 유효하다면
	 {
		if(PictureItemFlags[itemIdx] == false) // 플래그 인덱스가 false 라면 
		{
			PictureItemFlags[itemIdx] = true; //true로 만들고 
			PictureItemQueue.Enqueue(itemIdx); // 큐에 해당 인덱스 저장한다.
		}
	 }
}

void UGuide_GameInstance::SetInitInfo(TMap<FString,TMap<FString,FString>> data,TArray<FString> key)
{
	ElementDataMap = data;
	keyValue = key;
}

TMap<FString,FString> UGuide_GameInstance::SetMyDataText(FString myName)
{
	FString name;
	// 영어만 남기기
	for ( TCHAR Char : myName )
	{
		if ( 'a' <= Char && Char <= 'z' || 'A' <= Char && Char <= 'Z' ) // 알파벳 문자만 추가
		{
			name.AppendChar(Char);
		}
	}
	TMap<FString,FString> result = ElementDataMap[name];
	return result;
}

void UGuide_GameInstance::Create()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(FName("Room"));
			if(ExistingSession != nullptr)
			{
				SessionInterface->DestroySession(FName("Room"));
			}
			else
			{
				FOnlineSessionSettings SessionSettings;
				SessionSettings.bIsLANMatch = true;
				SessionSettings.NumPublicConnections = 5;
				SessionSettings.bShouldAdvertise = true;
				SessionSettings.bUsesPresence = true;

				SessionInterface->CreateSession(0,FName("Room"), SessionSettings);
			}
		}
	}
}

void UGuide_GameInstance::Find()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			SessionSearch->bIsLanQuery =true;
			SessionSearch->MaxSearchResults = 5;

			SessionInterface->FindSessions(0,SessionSearch.ToSharedRef());
		}
	}
}

void UGuide_GameInstance::Join()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid() && SessionSearch->SearchResults.Num()>0)
		{
			SessionInterface->JoinSession(0,FName("Room"), SessionSearch->SearchResults[0]);
		}
	}
}

void UGuide_GameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UWorld* world = GetWorld();
		if(world)
		{
			FString MapName = TEXT("Room?listen");
			world->ServerTravel(MapName);
		}
	}
}

void UGuide_GameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if(bWasSuccessful && SessionSearch.IsValid() && SessionSearch->SearchResults.Num()>0)
	{
		Join();
	}
}

void UGuide_GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if(Result == EOnJoinSessionCompleteResult::Success)
	{
		IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
		if(OnlineSubsystem)
		{
			IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
			if(SessionInterface.IsValid())
			{
				FString ConnectString;
				if(SessionInterface->GetResolvedConnectString(SessionName,ConnectString))
				{
					APlayerController* PlayerController = GetFirstLocalPlayerController();
					if(PlayerController)
					{
						PlayerController->ClientTravel(ConnectString,TRAVEL_Absolute);
					}
				}
			}
		}
	}
}

void UGuide_GameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		Create();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session: %s"), *SessionName.ToString());
	}
}
