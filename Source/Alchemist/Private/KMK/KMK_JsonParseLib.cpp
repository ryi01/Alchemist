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
	
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		FString parseDataList = response->GetStringField(TEXT("element"));
		FString parseDataList1 = response->GetStringField(TEXT("explanation"));
		// 원소 이름과 정보
		if(parseDataList.IsEmpty() ) result.Add(TEXT("element"), parseDataList);
		if( parseDataList1.IsEmpty() ) result.Add(TEXT("explanation"),parseDataList1);

		GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, FString::Printf(TEXT("AI Connect")));
	}

	return result;
}

TMap<FString, FString> UKMK_JsonParseLib::ResultAlchemistParsec(const FString& json, FString ResultAlchemist)
{
	TMap<FString, FString> result;

	return result;
}