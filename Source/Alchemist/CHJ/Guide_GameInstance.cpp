// Fill out your copyright notice in the Description page of Project Settings.


#include "Alchemist/CHJ/Guide_GameInstance.h"

#include <string>

#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "OnlineSessionSettings.h"
#include "SYH/SYH_QuizWaitWidget.h"
#include "KMK/KMK_GrabActorComp.h"

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
	// 세션 인터페이스 가져오기
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(); 
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			// 비동기 방식으로 움직임
			// 비동기의 델리게이트
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
	 FString tagName = ParsecItemName(PictureItems[PictureItems.Num()-1].Element_name);
	if(!correctionTag.Contains(tagName)) correctionTag.Add(tagName);
}

void UGuide_GameInstance::SetInitInfo(TMap<FString,TMap<FString,FString>> data,TArray<FString> key)
{
	ElementDataMap = data;
	keyValue = key;
}

TMap<FString,FString> UGuide_GameInstance::SetMyDataText(FString myName)
{
	if(ElementDataMap.IsEmpty() ) return TMap<FString, FString>();
	FString name = ParsecItemName(myName);
	TMap<FString,FString> result = ElementDataMap[name];
	return result;
}

FString UGuide_GameInstance::ParsecItemName(FString myName)
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
	
	return name;
}

void UGuide_GameInstance::Create()
{
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if(OnlineSubsystem)
	{
		IOnlineSessionPtr SessionInterface = OnlineSubsystem->GetSessionInterface();
		if(SessionInterface.IsValid())
		{
			FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(FName("Alpha"));
			if(ExistingSession != nullptr)
			{
				SessionInterface->DestroySession(FName("Alpha"));
			}
			else
			{
				FOnlineSessionSettings SessionSettings;
				if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL") // OnlineSubsystem 이 NULL 로 세팅되면 (NULL : 로컬 연결 설정)
				{
					SessionSettings.bIsLANMatch = true; // true 시 : 같은 네트워크에 있는 사람을 찾음 (로컬 연결 설정) 
				}

				else
				{
					SessionSettings.bIsLANMatch = false; // false 시 : 다른 네트워크와 연결 가능하도록 함. (Steam, XBox 등 공식플랫폼 연결 설정)
				}
				SessionSettings.NumPublicConnections = 5;// 플레이어 수
				SessionSettings.bShouldAdvertise = true;// 온라인에서 세션을 볼 수 있도록함. '광고한다'
				SessionSettings.bUsesPresence = true;// 로비기능을 활성화한다. (Host 하려면 필요)
				SessionSettings.bUseLobbiesIfAvailable = true; // 로비기능을 활성화한다. (Host 하려면 필요)
				SessionSettings.Set(FName("Player_Name"),StringBase64Encode(PlayerName),EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
				SessionInterface->CreateSession(0, FName("Alpha"), SessionSettings);
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
			SessionSearch->bIsLanQuery = false;
			SessionSearch->MaxSearchResults = 5;
			SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
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
			SessionInterface->JoinSession(0, FName("Alpha"), SessionSearch->SearchResults[0]);
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
			FString MapName = TEXT("Alpha?listen");
			world->ServerTravel(MapName);
		}
	}
}

void UGuide_GameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		int32 NumResults = SessionSearch->SearchResults.Num();
		UE_LOG(LogTemp, Warning, TEXT("Found %d sessions"), NumResults);

		if (NumResults > 0)
		{
			// 검색된 세션 정보를 출력
			for (int32 i = 0; i < NumResults; i++)
			{
				const FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[i];
				UE_LOG(LogTemp, Warning, TEXT("Session %d: %s"), i, *SearchResult.GetSessionIdStr());
			}
			Join();  // 첫 번째 세션에 Join
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("No sessions found"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Session search failed"));
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
						PlayerController->ClientTravel(ConnectString,TRAVEL_Absolute); // 열려있는 리슨서버로 강제로 이동시킴
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



// 보낼 때
FString UGuide_GameInstance::StringBase64Encode(const FString& str)
{
	// Set 할 때 : FString -> UTF8 (std::string) -> TArray<uint8> -> base64 로 Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()),
	utf8String.length());
	return FBase64::Encode(arrayData);
}

// 받을 때
FString UGuide_GameInstance::StringBase64Decode(const FString& str)
{
	// Get 할 때 : base64 로 Decode -> TArray<uint8> -> TCHAR
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string ut8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(ut8String.c_str());
}
