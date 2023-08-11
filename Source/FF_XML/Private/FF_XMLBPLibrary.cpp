// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_XMLBPLibrary.h"
#include "FF_XML.h"

// UE Includes.
#include "Kismet/GameplayStatics.h"

UFF_XMLBPLibrary::UFF_XMLBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UFF_XMLBPLibrary::XML_Doc_Create(UFFXMLDoc*& Out_Doc, FString CustomDeclaration, bool bAddDeclaration)
{
	Out_Doc = NewObject< UFFXMLDoc>();
    if (bAddDeclaration)
    {
        XMLDeclaration* Declaration = Out_Doc->Document.NewDeclaration(CustomDeclaration.IsEmpty() ? NULL : TCHAR_TO_UTF8(*CustomDeclaration));
        Out_Doc->Document.InsertFirstChild(Declaration);
    }
}

bool UFF_XMLBPLibrary::XML_Doc_Save(UPARAM(ref)UFFXMLDoc*& In_Doc, FString In_Path)
{
    if (!IsValid(In_Doc))
    {
        return false;
    }

    if (In_Path.IsEmpty())
    {
        return false;
    }

    FString Path = In_Path;
    FString PlatformName = UGameplayStatics::GetPlatformName();

    if (PlatformName == "Windows")
    {
        FPaths::MakePlatformFilename(Path);
    }

    else if (PlatformName == "Android")
    {

    }

    In_Doc->Document.SaveFile(TCHAR_TO_UTF8(*Path));

    return true;
}

bool UFF_XMLBPLibrary::XML_Doc_Print(UPARAM(ref)UFFXMLDoc*& In_Doc, FString& Out_String)
{
    if (!IsValid(In_Doc))
    {
        return false;
    }

    XMLPrinter Printer;
    In_Doc->Document.Print(&Printer);

    Out_String = Printer.CStr();

    return true;
}

bool UFF_XMLBPLibrary::XML_Element_Add_Map(UFFXMLElement*& Out_Element, UObject* Target, FString ElementName, FString ElementValue, TMap<FString, FString> Attributes)
{
    if (!IsValid(Target))
    {
        return false;
    }

    if (ElementName.IsEmpty())
    {
        return false;
    }

    TArray<FString> AttributeNames;
    Attributes.GenerateKeyArray(AttributeNames);
    
    UFFXMLElement* ChildElement = NewObject<UFFXMLElement>();
    UFFXMLElement* ParentElement = Cast<UFFXMLElement>(Target);

    if (IsValid(ParentElement))
    {
        ChildElement->Element = ParentElement->Element->InsertNewChildElement(TCHAR_TO_UTF8(*ElementName));
        
        if (!ElementValue.IsEmpty())
        {
            ChildElement->Element->SetText(TCHAR_TO_UTF8(*ElementValue));
        }

        for (int32 Index_Attributes = 0; Index_Attributes < AttributeNames.Num(); Index_Attributes++)
        {
            FString EachAttribute = AttributeNames[Index_Attributes];
            FString EachValue = *Attributes.Find(EachAttribute);
            ChildElement->Element->SetAttribute(TCHAR_TO_UTF8(*EachAttribute), TCHAR_TO_UTF8(*EachValue));
        }

        ParentElement->Element->InsertEndChild(ChildElement->Element);
    }

    UFFXMLDoc* ParentDocument = Cast<UFFXMLDoc>(Target);
    if (IsValid(ParentDocument))
    {
        ChildElement->Element = ParentDocument->Document.NewElement(TCHAR_TO_UTF8(*ElementName));
        
        if (!ElementValue.IsEmpty())
        {
            ChildElement->Element->SetText(TCHAR_TO_UTF8(*ElementValue));
        }
        
        for (int32 Index_Attributes = 0; Index_Attributes < AttributeNames.Num(); Index_Attributes++)
        {
            FString EachAttribute = AttributeNames[Index_Attributes];
            FString EachValue = *Attributes.Find(EachAttribute);
            ChildElement->Element->SetAttribute(TCHAR_TO_UTF8(*EachAttribute), TCHAR_TO_UTF8(*EachValue));
        }

        ParentDocument->Document.InsertEndChild(ChildElement->Element);
    }

    if (!IsValid(ParentElement) && !IsValid(ParentDocument))
    {
        return false;
    }

    Out_Element = ChildElement;

    return true;
}

bool UFF_XMLBPLibrary::XML_Element_Add_Single(UFFXMLElement*& Out_Element, UObject* Target, FString ElementName, FString ElementValue)
{
    if (!IsValid(Target))
    {
        return false;
    }

    UFFXMLElement* ChildElement = NewObject<UFFXMLElement>();
    UFFXMLElement* ParentElement = Cast<UFFXMLElement>(Target);

    if (IsValid(ParentElement))
    {
        ChildElement->Element = ParentElement->Element->InsertNewChildElement(TCHAR_TO_UTF8(*ElementName));
        
        if (!ElementValue.IsEmpty())
        {
            ChildElement->Element->SetText(TCHAR_TO_UTF8(*ElementValue));
        }
        
        ParentElement->Element->InsertEndChild(ChildElement->Element);
    }

    UFFXMLDoc* ParentDocument = Cast<UFFXMLDoc>(Target);
    if (IsValid(ParentDocument))
    {
        ChildElement->Element = ParentDocument->Document.NewElement(TCHAR_TO_UTF8(*ElementName));
        
        if (!ElementValue.IsEmpty())
        {
            ChildElement->Element->SetText(TCHAR_TO_UTF8(*ElementValue));
        }
        
        ParentDocument->Document.InsertEndChild(ChildElement->Element);
    }

    if (!IsValid(ParentElement) && !IsValid(ParentDocument))
    {
        return false;
    }

    Out_Element = ChildElement;

    return true;
}

bool UFF_XMLBPLibrary::XML_Element_Remove(UPARAM(ref)UFFXMLDoc*& In_Doc, UPARAM(ref)UFFXMLElement*& TargetElement)
{
    if (!IsValid(In_Doc))
    {
        return false;
    }

    In_Doc->Document.DeleteChild(TargetElement->Element);

    return true;
}

bool UFF_XMLBPLibrary::XML_Comment_Add(UObject* Target, FString In_Comment)
{
    if (!IsValid(Target))
    {
        return false;
    }

    XMLComment* CommentNode = nullptr;
    UFFXMLElement* Element = Cast<UFFXMLElement>(Target);

    if (IsValid(Element))
    {
        CommentNode = Element->Element->InsertNewComment(TCHAR_TO_UTF8(*In_Comment));
    }

    UFFXMLDoc* Document = Cast<UFFXMLDoc>(Target);
    if (IsValid(Document))
    {
        CommentNode = Document->Document.NewComment(TCHAR_TO_UTF8(*In_Comment));
        Document->Document.InsertEndChild(CommentNode);
    }

    if (!IsValid(Element) && !IsValid(Document))
    {
        return false;
    }

    return true;
}

bool UFF_XMLBPLibrary::XML_Node_Add(UPARAM(ref)UFFXMLDoc*& In_Doc, UFFXMLElement* Element_Parent, UFFXMLElement*& Element_Child, FString ElementName, TMap<FString, FString> In_String)
{
    if (!IsValid(In_Doc))
    {
        return false;
    }

    return true;
}