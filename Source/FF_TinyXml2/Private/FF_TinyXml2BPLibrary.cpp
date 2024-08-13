// Copyright Epic Games, Inc. All Rights Reserved.

#include "FF_TinyXml2BPLibrary.h"
#include "FF_TinyXml2.h"

UFF_TinyXml2BPLibrary::UFF_TinyXml2BPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

void UFF_TinyXml2BPLibrary::TinyXML2_Doc_Create(UFFTinyXml2_Doc*& Out_Doc, FString CustomDeclaration, bool bAddDeclaration)
{
	Out_Doc = NewObject< UFFTinyXml2_Doc>();
    if (bAddDeclaration)
    {
        XMLDeclaration* Declaration = Out_Doc->Document.NewDeclaration(CustomDeclaration.IsEmpty() ? NULL : TCHAR_TO_UTF8(*CustomDeclaration));
        Out_Doc->Document.InsertFirstChild(Declaration);
    }
}

bool UFF_TinyXml2BPLibrary::TinyXML2_Open_File(UFFTinyXml2_Doc*& Out_Doc, FString In_Path)
{
    if (In_Path.IsEmpty())
    {
        return false;
    }

    FString XML_Path = "";
    FILE* XML_File = nullptr;

#ifdef _WIN64

    XML_Path = In_Path;

    FPaths::MakeStandardFilename(XML_Path);
    if (!FPaths::FileExists(XML_Path))
    {
#pragma warning(disable : 6387)
        fclose(XML_File);
        return false;
    }

    FPaths::MakePlatformFilename(XML_Path);
    fopen_s(&XML_File, TCHAR_TO_UTF8(*XML_Path), "rb");

    if (!XML_File)
    {
        return false;
    }

#else

    XML_Path = FPlatformFileManager::Get().GetPlatformFile().ConvertToAbsolutePathForExternalAppForRead(*XML_Path);
    XML_File = fopen(TCHAR_TO_UTF8(*XML_Path), "rb");

    if (!XML_File)
    {
        return false;
    }

#endif // 

    Out_Doc = NewObject<UFFTinyXml2_Doc>();
    XMLError Result = Out_Doc->Document.LoadFile(XML_File);
    fclose(XML_File);

    if (Result == XML_SUCCESS)
    {
        return true;
    }

    else
    {
        Out_Doc = nullptr;
        return false;
    }
}

bool UFF_TinyXml2BPLibrary::TinyXML2_Open_Memory(UFFTinyXml2_Doc*& Out_Doc, TArray<uint8> In_Bytes)
{
    if (In_Bytes.Num() == 0)
    {
        return false;
    }

    const std::string XML_String(reinterpret_cast<const char*>(In_Bytes.GetData()), In_Bytes.Num());

    if (XML_String.empty())
    {
        return false;
    }

    Out_Doc = NewObject< UFFTinyXml2_Doc>();
    XMLError Result = Out_Doc->Document.Parse(XML_String.c_str());

    if (Result == XML_SUCCESS)
    {
        return true;
    }

    else
    {
        Out_Doc = nullptr;
        return false;
    }
}

bool UFF_TinyXml2BPLibrary::TinyXML2_Doc_Save(UPARAM(ref)UFFTinyXml2_Doc*& In_Doc, FString In_Path)
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

    In_Doc->Document.SaveFile(TCHAR_TO_UTF8(*Path));

    return true;
}

bool UFF_TinyXml2BPLibrary::TinyXML2_Doc_Print(UPARAM(ref)UFFTinyXml2_Doc*& In_Doc, FString& Out_String)
{
    if (!IsValid(In_Doc))
    {
        return false;
    }

    XMLPrinter Printer;
    In_Doc->Document.Print(&Printer);

    Out_String = UTF8_TO_TCHAR(Printer.CStr());

    return true;
}

bool UFF_TinyXml2BPLibrary::TinyXML2_Element_Add_Map(UFFTinyXml2_Element*& Out_Element, UObject* Target, FString ElementName, FString ElementValue, TMap<FString, FString> Attributes)
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
    
    UFFTinyXml2_Element* ChildElement = NewObject<UFFTinyXml2_Element>();
    UFFTinyXml2_Element* ParentElement = Cast<UFFTinyXml2_Element>(Target);

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

    UFFTinyXml2_Doc* ParentDocument = Cast<UFFTinyXml2_Doc>(Target);
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

bool UFF_TinyXml2BPLibrary::TinyXML2_Element_Add_Single(UFFTinyXml2_Element*& Out_Element, UObject* Target, FString ElementName, FString ElementValue)
{
    if (!IsValid(Target))
    {
        return false;
    }

    UFFTinyXml2_Element* ChildElement = NewObject<UFFTinyXml2_Element>();
    UFFTinyXml2_Element* ParentElement = Cast<UFFTinyXml2_Element>(Target);

    if (IsValid(ParentElement))
    {
        ChildElement->Element = ParentElement->Element->InsertNewChildElement(TCHAR_TO_UTF8(*ElementName));
        
        if (!ElementValue.IsEmpty())
        {
            ChildElement->Element->SetText(TCHAR_TO_UTF8(*ElementValue));
        }
        
        ParentElement->Element->InsertEndChild(ChildElement->Element);
    }

    UFFTinyXml2_Doc* ParentDocument = Cast<UFFTinyXml2_Doc>(Target);
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

bool UFF_TinyXml2BPLibrary::TinyXML2_Comment_Add(UFFTinyXml2_Comment*& Out_Comment, UObject* Target, FString In_Comment)
{
    if (!IsValid(Target))
    {
        return false;
    }

    XMLComment* CommentNode = nullptr;
    UFFTinyXml2_Element* Element = Cast<UFFTinyXml2_Element>(Target);

    if (IsValid(Element))
    {
        CommentNode = Element->Element->InsertNewComment(TCHAR_TO_UTF8(*In_Comment));
    }

    UFFTinyXml2_Doc* Document = Cast<UFFTinyXml2_Doc>(Target);
    if (IsValid(Document))
    {
        CommentNode = Document->Document.NewComment(TCHAR_TO_UTF8(*In_Comment));
        Document->Document.InsertEndChild(CommentNode);
    }

    if (!IsValid(Element) && !IsValid(Document))
    {
        return false;
    }

    Out_Comment = NewObject<UFFTinyXml2_Comment>();
    Out_Comment->Comment = CommentNode;

    return true;
}

bool UFF_TinyXml2BPLibrary::TinyXML2_Element_Remove(UPARAM(ref)UFFTinyXml2_Doc*& In_Doc, UPARAM(ref)UObject*& Target)
{
    if (!IsValid(In_Doc))
    {
        return false;
    }

    if (!IsValid(Target))
    {
        return false;
    }

    UFFTinyXml2_Element* TargetElement = Cast<UFFTinyXml2_Element>(Target);
    if (IsValid(TargetElement))
    {
        In_Doc->Document.DeleteNode(TargetElement->Element);
        Target = nullptr;

        return true;
    }

    UFFTinyXml2_Comment* TargetComment = Cast<UFFTinyXml2_Comment>(Target);
    if (IsValid(TargetComment))
    {
        In_Doc->Document.DeleteNode(TargetComment->Comment);
        Target = nullptr;

        return true;
    }

    else
    {
        return false;
    }
}