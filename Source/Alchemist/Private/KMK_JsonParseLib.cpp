// Fill out your copyright notice in the Description page of Project Settings.


#include "KMK_JsonParseLib.h"

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
TMap<FString, FString> UKMK_JsonParseLib::ChatBotParsec(const FString& json, FString ingredient)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

	TMap<FString, FString> result;
	
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		/*FString menu = response->GetStringField(TEXT("food_name"));
		FString recipe = response->GetStringField(TEXT("recipe"));*/
		GEngine->AddOnScreenDebugMessage(2, 5, FColor::Blue, FString::Printf(TEXT("AI Connect")));
	}

	// json 파일을 오브젝트 형식으로 된 배열형태로 받은상태
	//FString returnValue;
	//if ( FJsonSerializer::Deserialize(reader, response) )
	//{
	//	TArray<TSharedPtr<FJsonValue>> parseDataList = response->GetArrayField(TEXT("items"));
	//	for ( TSharedPtr<FJsonValue> data : parseDataList )
	//	{
	//		// 책의 이름과 저자
	//		FString bookName = data->AsObject()->GetStringField("bk_nm");
	//		FString authorName = data->AsObject()->GetStringField("aut_nm");
	//		returnValue.Append(FString::Printf(TEXT("BookName : %s / AuthorName : %s\n"), *bookName, *authorName));
	//	}
	//}
	return result;
}

TMap<FString, FString> UKMK_JsonParseLib::ResultAlchemistParsec(const FString& json, FString ResultAlchemist)
{
	TMap<FString, FString> result;

	return result;
}