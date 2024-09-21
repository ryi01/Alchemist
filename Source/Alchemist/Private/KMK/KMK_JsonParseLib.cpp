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
            result = SplitSection(parseDataList1, Sections);
            if ( result.IsEmpty() )
            {
                TMap<FString,FString> failedInfo;
                failedInfo.Add(TEXT("Fail"), parseDataList1);
                result.Add(TEXT("Fail"),failedInfo);
            }
		}
	}
	return result;
}


TMap<FString, FString> UKMK_JsonParseLib::ResultAlchemistParsec(const FString& json)
{
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
    TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());
	TMap<FString, FString> result;
    if ( FJsonSerializer::Deserialize(reader,response) )
    {

        FString stringValue = response->GetStringField(TEXT("combinable"));
        if ( response->TryGetStringField(TEXT("combinable"),stringValue) && !stringValue.IsEmpty() )
        {
            bool isCreate = stringValue.Equals(TEXT("true"),ESearchCase::IgnoreCase);
            if ( isCreate )
            {
                FString FinalEle = response->GetStringField(TEXT("resultElement"));
                FString EleName = response->GetStringField(TEXT("fullName"));
                FString UsingEle = response->GetStringField(TEXT("uses"));

                result.Add(TEXT("Result"),FinalEle);
                result.Add(TEXT("Name"),EleName);
                result.Add(TEXT("Using"),UsingEle);
            }
        }
        else result.IsEmpty();

    }
	return result;
}
TMap<FString,TMap<FString,FString>> UKMK_JsonParseLib::InitInfoParsec(const FString& json,TArray<FString> Sections)
{
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
    TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

    TMap<FString,TMap<FString,FString>> result;
    // 섹션별로 파싱 (SplitSection 함수 호출)
    // TMap<FString,FString> ElementRes = SplitSectionLight(json,Sections);
    
    // 최종 result에 추가
    //result.Add(name,ElementRes);

    if ( FJsonSerializer::Deserialize(reader,response) )
    {
        const TArray<TSharedPtr<FJsonValue>>* ElementsArray;
        if ( response->TryGetArrayField(TEXT("elements"),ElementsArray) )
        {
            for ( const TSharedPtr<FJsonValue>& ElementValue : *ElementsArray )
            {
                TSharedPtr<FJsonObject> eleObject = ElementValue->AsObject();
                if ( eleObject.IsValid() )
                {
                   FString name = eleObject->GetStringField(TEXT("element_base"));
                   FString infoString = eleObject->GetStringField(TEXT("base_info"));
                   if ( !name.IsEmpty() && !infoString.IsEmpty() )
                   {
                        // 섹션별로 파싱 (SplitSection 함수 호출)
                        TMap<FString,FString> ElementRes = SplitSectionLight(infoString,Sections);

                        // 최종 result에 추가
                        result.Add(name,ElementRes);
                   }
                }
            }
        }
    }
    return result;
}

TMap<FString,TMap<FString,FString>> UKMK_JsonParseLib::RecommandEleParsec(const FString& json)
{
    TSharedRef<TJsonReader<TCHAR>> reader = TJsonReaderFactory<TCHAR>::Create(json);
    TSharedPtr<FJsonObject> response = MakeShareable(new FJsonObject());

    TMap<FString,TMap<FString,FString>> result;
    if ( FJsonSerializer::Deserialize(reader,response) )
    {
        // 'similar' 필드에서 텍스트 가져오기
        FString InputText;

        if ( response->TryGetStringField(TEXT("similar"),InputText) )
        {
            // 1. 먼저 각 항목을 1., 2., 3.으로 나눔
            TArray<FString> Sections;
            InputText.ParseIntoArray(Sections,TEXT("\n\n"),true); // 두 개의 개행으로 구분

            for ( int32 i = 0; i < Sections.Num(); ++i ) // 첫 번째 설명을 제외하기 위해 인덱스를 1로 시작
            {

                FString Section = Sections[ i ].TrimStartAndEnd(); // 각 섹션의 앞뒤 공백 제거

                // 각 섹션의 제목을 추출 (예: "1. 리튬 (Lithium, Li)")
                FString SectionTitle;
                int32 SectionTitleEnd = Section.Find(TEXT("\n"));
                if ( SectionTitleEnd != INDEX_NONE )
                {
                    SectionTitle = Section.Left(SectionTitleEnd).TrimStartAndEnd();
                    // 섹션 제목 다음 부분만 남김
                    Section = Section.Mid(SectionTitleEnd + 1).TrimStartAndEnd();
                }
                else
                {
                    SectionTitle = Section.TrimStartAndEnd();
                }

                // 2. a), b)로 나누기
                TArray<FString> SubSections;
                Section.ParseIntoArray(SubSections,TEXT("   b)"),true); // b)로 나눔

                if ( SubSections.Num() == 2 )
                {
                    FString ASection = SubSections[ 0 ].Replace(TEXT("   a)"),TEXT("a)")).TrimStartAndEnd(); // a) 부분 정리
                    FString BSection = SubSections[ 1 ].TrimStartAndEnd(); // b) 부분 앞뒤 공백 제거

                    // 3. 하위 섹션을 TMap으로 저장
                    TMap<FString,FString> SubMap;
                    SubMap.Add(TEXT("a)"),ASection);
                    SubMap.Add(TEXT("b)"),BSection);

                    // 4. 전체 결과에 추가
                    result.Add(SectionTitle,SubMap);
                }
            }
        }
    }
    return result;
}



#pragma region SplitString
TMap<FString,FString> UKMK_JsonParseLib::SplitSectionLight(const FString& json,TArray<FString> Sections)
{
    int32 StartIndex = 0;
    TMap<FString,FString> result;

    for ( int32 i = 0; i < Sections.Num(); i++ )
    {
        // 현재 섹션의 시작 위치 찾기
        int32 SectionStart = json.Find(Sections[ i ],ESearchCase::IgnoreCase,ESearchDir::FromStart,StartIndex);

        if ( SectionStart != INDEX_NONE )
        {
            // 다음 섹션의 시작 위치 찾기
            int32 NextSectionStart = ( i + 1 < Sections.Num() )
                ? json.Find(Sections[ i + 1 ],ESearchCase::IgnoreCase,ESearchDir::FromStart,SectionStart + Sections[ i ].Len())
                : json.Len();

            // 섹션 내용 추출
            FString SectionContent = json.Mid(SectionStart,NextSectionStart - SectionStart).TrimStartAndEnd();

            // 섹션 제목을 제거한 실제 내용만 추출하여 저장
            int32 ContentStart = Sections[ i ].Len(); // 제목의 길이만큼 건너뛰기
            result.Add(Sections[ i ],SectionContent.Mid(ContentStart).TrimStartAndEnd());

            // 다음 섹션으로 이동
            StartIndex = NextSectionStart;
        }
    }
    return result;
}

TMap<FString,TMap<FString,FString>> UKMK_JsonParseLib::SplitSection(const FString& json,TArray<FString> Sections)
{
#pragma region StringParsec
    int32 StartIndex = 0;
    TMap<FString,TMap<FString,FString>> result;
    // 첫 번째 섹션의 시작 위치를 찾습니다.
    int32 FirstSectionStart = json.Find(Sections[ 0 ],ESearchCase::IgnoreCase,ESearchDir::FromStart);
    if ( FirstSectionStart != INDEX_NONE )
    {
        // 첫 번째 섹션 이전의 내용을 "Header"로 저장
        FString HeaderContent = json.Left(FirstSectionStart).TrimStartAndEnd();
        TMap<FString,FString> HeaderDetails;
        HeaderDetails.Add(TEXT("Content"),HeaderContent);
        result.Add(TEXT("Header"),HeaderDetails);

        // StartIndex를 첫 번째 섹션 시작 위치로 설정
        StartIndex = FirstSectionStart;
    }
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

            // 모든 줄바꿈 문자를 통일
            SectionContent.ReplaceInline(TEXT("\\n"),TEXT("\n"));
            SectionContent.ReplaceInline(TEXT("\r\n"),TEXT("\n"));  // \r\n을 \n으로 변경
            SectionContent.ReplaceInline(TEXT("\r"),TEXT("\n"));    // \r도 \n으로 변경

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
            if ( i != Sections.Num() - 1 )StartIndex = NextSectionStart;

        }
    }
    UE_LOG(LogTemp,Warning,TEXT("String Length: %d, %d"),StartIndex,json.Len());
    // 마지막 섹션 이후의 텍스트를 "Footer"로 저장
    if ( StartIndex < json.Len() )
    {
        // json의 StartIndex부터 FooterContent를 추출
        FString FooterContent = json.Mid(StartIndex);

        // '\n\n'을 기준으로 마지막 섹션의 시작을 찾는다
        int32 FooterStartIndex = FooterContent.Find(TEXT("\n\n"),ESearchCase::IgnoreCase,ESearchDir::FromEnd);

        // FooterStartIndex 이후의 텍스트만 사용
        if ( FooterStartIndex != INDEX_NONE )
        {
            FooterContent = FooterContent.Mid(FooterStartIndex + 2).TrimStartAndEnd(); // '\n\n' 이후의 텍스트 추출
        }
        else
        {
            // 만약 '\n\n'을 찾지 못하면 전체 내용을 Trim해서 사용
            FooterContent.Empty();
        }

        // 로그로 FooterContent의 상태를 출력해서 디버깅
        UE_LOG(LogTemp,Warning,TEXT("FooterContent: %s"),*FooterContent);

        if ( !FooterContent.IsEmpty() ) // Footer 내용이 비어있지 않을 경우에만 추가
        {
            TMap<FString,FString> FooterDetails;
            FooterDetails.Add(TEXT("Content"),FooterContent);
            result.Add(TEXT("Footer"),FooterDetails);
        }
    }
    return result;
#pragma endregion
}
#pragma endregion


