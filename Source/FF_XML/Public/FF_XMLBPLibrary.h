// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

THIRD_PARTY_INCLUDES_START
#include "tinyxml2.h"
THIRD_PARTY_INCLUDES_END

#include "FF_XMLBPLibrary.generated.h"

using namespace tinyxml2;

/*
*	PDFium Binaries
*	https://github.com/leethomason/tinyxml2
*/

UCLASS(BlueprintType)
class FF_XML_API UFFXMLDoc : public UObject
{
	GENERATED_BODY()

public:

	XMLDocument Document;

};

UCLASS(BlueprintType)
class FF_XML_API UFFXMLElement : public UObject
{
	GENERATED_BODY()

public:

	XMLElement* Element = nullptr;

};

UCLASS(BlueprintType)
class FF_XML_API UFFXMLComment : public UObject
{
	GENERATED_BODY()

public:

	XMLComment* Comment = nullptr;

};

UCLASS()
class UFF_XMLBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Create Document", Keywords = "xml, document, create"), Category = "FF_XML")
	static void XML_Doc_Create(UFFXMLDoc*& Out_Doc, FString CustomDeclaration, bool bAddDeclaration = true);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Save Document", Keywords = "xml, document, save"), Category = "FF_XML")
	static bool XML_Doc_Save(UPARAM(ref)UFFXMLDoc*& In_Doc, FString In_Path);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Print Document", Keywords = "xml, document, save"), Category = "FF_XML")
	static bool XML_Doc_Print(UPARAM(ref)UFFXMLDoc*& In_Doc, FString& Out_String);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Add Element (MAP)", Keywords = "xml, document, element, child, add"), Category = "FF_XML")
	static bool XML_Element_Add_Map(UFFXMLElement*& Out_Element, UObject* Target, FString ElementName, FString ElementValue, TMap<FString, FString> Attributes);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Add Element (Single Value)", Keywords = "xml, document, element, child, add"), Category = "FF_XML")
	static bool XML_Element_Add_Single(UFFXMLElement*& Out_Element, UObject* Target, FString ElementName, FString ElementValue);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Add Comment", Keywords = "xml, document, comment, add"), Category = "FF_XML")
	static bool XML_Comment_Add(UFFXMLComment*& Out_Comment, UObject* Target, FString In_Comment);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XML - Remove Element", Keywords = "xml, document, element, child, add"), Category = "FF_XML")
	static bool XML_Element_Remove(UPARAM(ref)UFFXMLDoc*& In_Doc, UPARAM(ref)UObject*& Target);

};
