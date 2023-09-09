// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START
#include "tinyxml2.h"
THIRD_PARTY_INCLUDES_END

#include "FF_TinyXml2BPLibrary.generated.h"

using namespace tinyxml2;

/*
*	TinyXml2 Binaries
*	https://github.com/leethomason/tinyxml2
*/

UCLASS(BlueprintType)
class FF_TINYXML2_API UFFTinyXml2_Doc : public UObject
{
	GENERATED_BODY()

public:

	XMLDocument Document;

};

UCLASS(BlueprintType)
class FF_TINYXML2_API UFFTinyXml2_Element : public UObject
{
	GENERATED_BODY()

public:

	XMLElement* Element = nullptr;

};

UCLASS(BlueprintType)
class FF_TINYXML2_API UFFTinyXml2_Comment : public UObject
{
	GENERATED_BODY()

public:

	XMLComment* Comment = nullptr;

};

UCLASS()
class UFF_TinyXml2BPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Create Document", Keywords = "tinyxml2, xml, document, create"), Category = "FF_TinyXml2")
	static void TinyXML2_Doc_Create(UFFTinyXml2_Doc*& Out_Doc, FString CustomDeclaration, bool bAddDeclaration = true);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Open File", Keywords = "tinyxml2, xml, document, open, file"), Category = "FF_TinyXml2")
	static bool TinyXML2_Open_File(UFFTinyXml2_Doc*& Out_Doc, FString XML_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Open From Memory", Keywords = "tinyxml2, xml, document, open, memory, buffer, bytes"), Category = "FF_TinyXml2")
	static bool TinyXML2_Open_Memory(UFFTinyXml2_Doc*& Out_Doc, TArray<uint8> In_Bytes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Save Document", Keywords = "tinyxml2, xml, document, save"), Category = "FF_TinyXml2")
	static bool TinyXML2_Doc_Save(UPARAM(ref)UFFTinyXml2_Doc*& In_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Print Document", Keywords = "tinyxml2, xml, document, save"), Category = "FF_TinyXml2")
	static bool TinyXML2_Doc_Print(UPARAM(ref)UFFTinyXml2_Doc*& In_Doc, FString& Out_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Add Element (MAP)", Keywords = "tinyxml2, xml, document, element, child, add"), Category = "FF_TinyXml2")
	static bool TinyXML2_Element_Add_Map(UFFTinyXml2_Element*& Out_Element, UObject* Target, FString ElementName, FString ElementValue, TMap<FString, FString> Attributes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Add Element (Single Value)", Keywords = "tinyxml2, xml, document, element, child, add"), Category = "FF_TinyXml2")
	static bool TinyXML2_Element_Add_Single(UFFTinyXml2_Element*& Out_Element, UObject* Target, FString ElementName, FString ElementValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Add Comment", Keywords = "tinyxml2, xml, document, comment, add"), Category = "FF_TinyXml2")
	static bool TinyXML2_Comment_Add(UFFTinyXml2_Comment*& Out_Comment, UObject* Target, FString In_Comment);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TinyXML2 - Remove Element", Keywords = "tinyxml2, xml, document, element, child, add"), Category = "FF_TinyXml2")
	static bool TinyXML2_Element_Remove(UPARAM(ref)UFFTinyXml2_Doc*& In_Doc, UPARAM(ref)UObject*& Target);

};
