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
TMap<FString,TMap<FString,FString>> UKMK_JsonParseLib::ChatBotParsec(const FString& json,TArray<FString> Sections)
{
	TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
	TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

    TMap<FString,TMap<FString,FString>> result;
  
	if ( FJsonSerializer::Deserialize(reader, response) )
	{
		FString parseDataList1 = response->GetStringField(TEXT("explanation"));
		if ( response->TryGetStringField(TEXT("explanation"),parseDataList1) && !parseDataList1.IsEmpty() )
		{
#pragma region StringParsec
            int32 StartIndex = 0;
            for ( int32 i = 0; i < Sections.Num(); i++ )
            {
                // 현재 섹션의 시작 위치 찾기
                int32 SectionStart = json.Find(Sections[ i ],ESearchCase::IgnoreCase,ESearchDir::FromStart,StartIndex);

                if ( SectionStart != INDEX_NONE )
                {
                    // 다음 섹션의 시작 위치 찾기
                    int32 NextSectionStart = ( i + 1 < Sections.Num() ) ? json.Find(Sections[ i + 1 ],ESearchCase::IgnoreCase,ESearchDir::FromStart,SectionStart + 1) : json.Len();

                    // 섹션 내용 추출
                    FString SectionContent = json.Mid(SectionStart,NextSectionStart - SectionStart).TrimStartAndEnd();

                    // 각 항목을 저장할 TMap 생성
                    TMap<FString,FString> SectionDetails;

                    // 섹션 내용을 줄 단위로 분리
                    TArray<FString> Lines;
                    SectionContent.ParseIntoArray(Lines,TEXT("\n"),true);

                    for ( FString& Line : Lines )
                    {
                        Line = Line.TrimStartAndEnd();

                        // 정보 항목을 "-" 기호를 기준으로 분리
                        if ( Line.StartsWith(TEXT("- ")) )
                        {
                            int32 DelimIndex;

                            if ( Line.FindChar(TEXT(':'),DelimIndex) )
                            {
                                FString Key = Line.Mid(2,DelimIndex - 2).TrimStartAndEnd();  // "- "를 제거하고 키 추출
                                FString Value = Line.Mid(DelimIndex + 1).TrimStartAndEnd();   // ":" 이후를 값으로 저장

                                SectionDetails.Add(Key,Value);
                            }
                        }
                    }

                    // 전체 섹션을 TMap에 추가
                    result.Add(Sections[ i ],SectionDetails);

                    // 다음 섹션의 시작 인덱스로 이동
                    StartIndex = NextSectionStart;
                }
            }

#pragma endregion
		}
	}
	return result;
}

TMap<FString, FString> UKMK_JsonParseLib::ResultAlchemistParsec(const FString& json, FString ResultAlchemist)
{
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
    TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());
	TMap<FString, FString> result;
    if ( FJsonSerializer::Deserialize(reader,response) )
    {
        FString stringValue = response->GetStringField(TEXT("조합 가능 여부"));
        bool isCreate = stringValue.Equals(TEXT("true"), ESearchCase::IgnoreCase);
        if (!isCreate)
        {
            return;
        }
        FString FinalEle = response->GetStringField(TEXT("결과원소"));
        FString EleName = response->GetStringField(TEXT("이름"));
        FString UsingEle = response->GetStringField(TEXT("사용처"));

        result.Add(TEXT("Final"), FinalEle);
        result.Add(TEXT("Name"),EleName);
        result.Add(TEXT("Using"),UsingEle);
    }
	return result;
}