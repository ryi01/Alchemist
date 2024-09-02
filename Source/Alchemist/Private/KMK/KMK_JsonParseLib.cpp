// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK/KMK_JsonParseLib.h"

FString UKMK_JsonParseLib::MakeJson(const TMap<FString, FString> source)
{
	// source를 JsonObject 형식으로 만든다.
	TSharedPtr<FJsonObject> jsonObject = MakeShareable(new FJsonObject());

	for ( TPair<FString, FString> pair : source )
	{
		jsonObject->SetStringField(pair.Key, pair.Value);
	}

	// writer를 만들어서 JsonObject를 인코딩해서 
	FString json;
	TSharedRef<TJsonWriter<TCHAR>> writer = TJsonWriterFactory<TCHAR>::Create(&json);
	FJsonSerializer::Serialize(jsonObject.ToSharedRef(), writer);
	// 반환한다.
	return json;
}
// 챗봇의 값 파섹
TMap<FString, FString> UKMK_JsonParseLib::ChatBotParsec(const FString& json, FString ResultChatBot)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	TMap<FString, FString> result;
	TArray<FString> Sections = { TEXT("1. 기본 정보"), TEXT("2. 특성"), TEXT("3. 용도"), TEXT("4. 흥미로운 사실") };
	int32 index = 0;

	for ( int32 i = 0; i < Sections.Num(); i++ )
	{
		// 현재 섹션의 시작 위치 찾기
		int32 SectionStart = json.Find(Sections[ i ],ESearchCase::IgnoreCase,ESearchDir::FromStart,index);

		if ( SectionStart != INDEX_NONE )
		{
			// 다음 섹션의 시작 위치 찾기
			int32 NextSectionStart = ( i + 1 < Sections.Num() ) ? json.Find(Sections[ i + 1 ],ESearchCase::IgnoreCase,ESearchDir::FromStart,SectionStart + 1) : json.Len();

			// 섹션 내용 추출
			FString SectionContent = json.Mid(SectionStart,NextSectionStart - SectionStart).TrimStartAndEnd();

			// 섹션 제목을 제거한 실제 내용만 추출하여 저장
			int32 ContentStart = SectionContent.Find(TEXT(":")) + 1;
			if ( ContentStart > 0 )
			{
				result.Add(Sections[ i ],SectionContent.Mid(ContentStart).TrimStartAndEnd());
			}

			// 다음 섹션으로 이동
			index = NextSectionStart;
		}
	}
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		FString parseDataList = response->GetStringField(TEXT("element"));
		FString parseDataList1 = response->GetStringField(TEXT("explanation"));
		// 원소 이름과 정보
		if( response->TryGetStringField(TEXT("element"),parseDataList) && !parseDataList.IsEmpty() ) result.Add(TEXT("element"), parseDataList);
		if ( response->TryGetStringField(TEXT("explanation"),parseDataList1) && !parseDataList1.IsEmpty() )
		{
			
		}
	}
	return result;
}

TMap<FString, FString> UKMK_JsonParseLib::ResultAlchemistParsec(const FString& json, FString ResultAlchemist)
{
	TMap<FString, FString> result;

	return result;
}